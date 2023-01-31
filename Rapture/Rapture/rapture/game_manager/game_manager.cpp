#include "game_manager.h"
#include "game_manager_def.h"

// stdテンプレート
#include <vector>
#include <string>

// DirectX12
#include <d3d12.h>
#include <dxgi1_6.h> // DirectX Graphics Infrastructure // 
/* DirectX APIよりもドライバー側により近いAPIらしい */
/* DXGI1_6だとVisualStudioGraphicsDebuggerが誤動作する場合があるらしい */
#include <d3dcompiler.h>
#include <DirectXMath.h>

//#pragma comment(lib,"d3d12.lib")
//#pragma comment(lib,"dxgi.lib")
//#pragma comment(lib,"d3dcompiler.lib")

#include <Bifrost/Subsystem/Dominator/SubsystemDominator.h>
#include <Bifrost/Subsystem/Resource/ResourceManagementSubsystem.h>
#include <Bifrost/Subsystem/Rendering/RenderingSubsystem.h>
#include <Bifrost/Subsystem/Rendering/RenderingSubsystemInitializer.h>
#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>

#include <Atlantis/RHIProccessor/RHIProcessor.h>

#include <Atlantis/Resource/TextureResourceManager.h>
#include <Atlantis/Resource/ShaderManager.h>
#include <Bifrost/Resource/Manager/MeshDataManager.h>
#include <Bifrost/Resource/Manager/MaterialManager.h>
#include <Bifrost/Resource/Manager/PipelineStateObjectManager.h>
#include <rapture/Subsystem/ResourceSubsystemImpl.h>

#include <Bifrost/Subsystem/Updater/UpdateProcessorSubsystem.h>

#include <rapture/Environment/ResourceTypeDefine.h>

#include <Bifrost/Resource/DefaultResourceDefine.h>

#include <Atlantis/DirectX12/DirectXTex/DirectXTex.h>
#include <Atlantis/DirectX12/DirectXHelper/d3dx12.h>

#include <eden/include/utility/ender_utility.h>

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>
#include <Atlantis/DirectX12/GlueMath.h>

#include <Atlantis/DirectX12/MainDevice/MainDevice.h>
#include <Atlantis/DirectX12/DirectXDebug/DirectXDebug.h>
#include <Atlantis/DirectX12/Command/CommandContext.h>
#include <Atlantis/DirectX12/Command/CommandQueue.h>
#include <Atlantis/DirectX12/SwapChain/SwapChain.h>
#include <Atlantis/DirectX12/RenderTargetView/RenderTargetView.h>
#include <Atlantis/DirectX12/DepthStencilView/DepthStencilView.h>
#include <Atlantis/DirectX12/Fence/Fence.h>
#include <Atlantis/DirectX12/Barrier/Barrier.h>

#include <Atlantis/DirectX12/VertexBuffer/VertexBuffer.h>
#include <Atlantis/DirectX12/IndexBuffer/IndexBuffer.h>

#include <Bifrost/Model/MeshData/MeshData.h>
#include <Bifrost/Model/MeshData/MeshDataInitializer.h>

#include <Atlantis/DirectX12/Viewport/Viewport.h>
#include <Atlantis/DirectX12/ScissorRect/ScissorRect.h>

#include <Atlantis/DirectX12/Shader/GraphicsShader.h>
#include <Atlantis/DirectX12/Shader/GraphicsShaderInitializer.h>

#include <Atlantis/DirectX12/Texture/Texture.h>
#include <Atlantis/DirectX12/Texture/TextureResource.h>
#include <Atlantis/DirectX12/Texture/TextureResourceInitializer.h>

#include <Atlantis/DirectX12/GraphicsPipeline/GraphicsPipeline.h>
#include <Atlantis/DirectX12/RootSignature/RootSignature.h>

#include <eden/include/gadget/FileLoader/FileLoader.h>

#include <rapture/Test/Test.h>
#include <eden/include/math/random_utility.h>

#include <eden/include/utility/StringUtility.h>

#include <Bifrost/Model/Pmd/PmdMaterialData.h>
#include <Bifrost/Resource/PSO/PipelineStateObject.h>

#include <Bifrost/Model/Component/PmdModelComponent.h>
#include <Bifrost/Model/Pmd/PmdMaterialDefine.h>
#include <Atlantis/SceneView/SceneView.h>

using namespace std;
using namespace DirectX;

EDENS_NAMESPACE_USING;
USING_ATLANTIS;
USING_BIFROST;

using namespace Glue;


// ビュー行列
XMMATRIX ViewMatrix = {};
// プロジェクション行列
XMMATRIX ProjectionMatrix = {};

// 頂点数
uint32 PmdVertexNum = 0;
uint32 PmdIndexNum = 0;

// コンスタントバッファ
struct SceneMatricesData
{
	FMatrix World = {};
	FMatrix View = {};
	FMatrix ViewProjection = {};
	FMatrix WorldViewProjection = {};
	Vector3 EyePosition = {};
	float padding1 = 0.f;
	Vector3 LightPosition = {};
	float padding2 = 0.f;
	Vector3 LightColor = {};
	float padding3 = 0.f;
};

// シェーダーに送るマテリアルデータ
struct MaterialForHlsl
{
	Vector3 Diffuse = {};
	float Alpha = 0.f;
	Vector3 SpecularColor = {};
	float Specularity = 0.f;
	Vector3 AmbientColor = {};
	//float ambientAlpha = 0.f;
};

// それ以外のマテリアルデータ
struct AdditionalMaterial
{
	Hash160 TexPathHash = {};
	uint8 ToonIndex = 0;
	bool EdgeFlag = false;
};

// マテリアルデータ
struct MaterialData
{
	uint32 IndiciesNum = 0;
	MaterialForHlsl gpuData = {};
	AdditionalMaterial addData = {};
};

constexpr uint32 MaterialDataSize = sizeof(MaterialData);

vector<MaterialData> m_MaterialData = {};

FSceneData SceneData = {};

#define D3D_ERROR_CHECK_TEMP(result) if(FAILED(result)) { return false; };

b8 CGameManager::Initialize(FGameManagerInitializer * _Initializer)
{
	/// <summary>
	///  SHA1 Test
	/// </summary>
	/// <param name="_Initializer"></param>
	/// <returns></returns>
	//Test::SHA1TestMain();

	// DirectX12を初期化 //

	do 
	{
#ifdef RENDER_TEST
		//m_Debug = new CDirectXDebug();
		//if (!m_Debug) { break; }
		//m_Debug->EnableDebugLayer();

		m_MainDevice = new CDX12MainDevice();
		if (!m_MainDevice) { break; };
		if (!m_MainDevice->Initialize()){ break;}

		m_CommandContext = new CCommandContext();
		if (!m_CommandContext) { break; };

		CCommandContext::FCommandContextInitializer cmdInitializer;
		cmdInitializer.Device = m_MainDevice->GetDevice();
		cmdInitializer.listType = Glue::COMMAND_LIST_TYPE_DIRECT;
		cmdInitializer.NodeMask = 0;

		if (!m_CommandContext->Initialize(cmdInitializer))
		{
			break;
		}


		m_CommandQueue = new CCommandQueue();
		if (!m_CommandQueue) { break; };

		CCommandQueue::FCommandQueueInitializer queueInitializer;
		queueInitializer.Device = m_MainDevice->GetDevice();
		queueInitializer.listType = cmdInitializer.listType;
		queueInitializer.QueueFlag = Glue::COMMAND_QUEUE_FLAG_NONE;
		queueInitializer.QueuePriority = Glue::COMMAND_QUEUE_PRIORITY_NORMAL;

		if (!m_CommandQueue->Initialize(queueInitializer))
		{
			break;
		}


		m_SwapChain = new CSwapChain();
		if (!m_SwapChain) { break; }
	
		CSwapChain::FSwapChainInitializer swapchainInitializer;
		swapchainInitializer.Device = m_MainDevice->GetDevice();
		swapchainInitializer.Factory = m_MainDevice->GetGIFactory();
		swapchainInitializer.Queue = m_CommandQueue->GetCommandQueue();
		swapchainInitializer.ViewportWidth = _Initializer->ViewportWidth;
		swapchainInitializer.ViewportHeight = _Initializer->ViewportHeight;
		swapchainInitializer.WindowHandle = _Initializer->WindowHandle;
		swapchainInitializer.BufferCount = 2;

		if (!m_SwapChain->Initialize(swapchainInitializer))
		{
			break;
		}

		{
			D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			//rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

			m_RTV.resize(swapchainInitializer.BufferCount);
			
			CRenderTargetView::FRenderTargetViewInitializer initializer;
			initializer.Device = m_MainDevice->GetDevice();
			//initializer.RtvDesc = &rtvDesc;
			initializer.Format = EDataFormat::FORMAT_R8G8B8A8_UNORM;
			initializer.RTVDimension = ERTVDimension::RTV_DIMENSION_TEXTURE2D;

			for (uint32 i = 0; i < m_RTV.size(); ++i)
			{
				m_RTV[i] = new CRenderTargetView();
				initializer.ResPtr = m_SwapChain->GetBackBuffer()->at(i);
				m_RTV[i]->Initialize(initializer);
			}
		}

		{
			CDepthStencilView::FInitializer initializer = {};
			initializer.Device = m_MainDevice->GetDevice();
			initializer.Width = _Initializer->ViewportWidth;
			initializer.Height = _Initializer->ViewportHeight;
			initializer.ClearDepth = 1.f;


			m_DepthStencilView = new CDepthStencilView();
			D3D_INIT_PROCESS_CHECK(m_DepthStencilView->Initialize(initializer));

		}

		{
			m_Barrier = new CBarrier();
			D3D_INIT_PROCESS_CHECK(m_Barrier);
			CBarrier::FInitializer initializer = {};
			initializer.BarrierType = Glue::EResourceBarrierType::BARRIER_TYPE_TRANSITION;
			m_Barrier->Initializer(initializer);
		}

		{
			m_Fence = new CFence();
			if (!m_Fence) { break; }

			CFence::FFenceInitializer fenceInitializer;
			fenceInitializer.Device = m_MainDevice->GetDevice();

			if (!m_Fence->Initialize(fenceInitializer))
			{
				break;
			}
		}

		{
			m_Viewport = new CViewport();
			if (!m_Viewport) { break; };

			CViewport::FViewportInitializer viewportInitializer;
			viewportInitializer.ViewportWidth = SCast<float>(_Initializer->ViewportWidth);
			viewportInitializer.VIewportHeight = SCast<float>(_Initializer->ViewportHeight);
			if (!m_Viewport->Initialize(viewportInitializer))
			{
				break;
			}
		}

		{
			m_ScissorRect = new CScissorRect();
			CHECK_RESULT_BREAK(m_ScissorRect);

			CScissorRect::FInitializer initializer = {};
			initializer.RectWidth = _Initializer->ViewportWidth;
			initializer.RectHeight = _Initializer->ViewportHeight;

			CHECK_RESULT_BREAK(m_ScissorRect->Initialize(initializer));
			
		}
#endif

		{
			m_SubsystemDominator = new CSubsystemDominator();
			CHECK_RESULT_BREAK(m_SubsystemDominator);

			CHECK_RESULT_BREAK(m_SubsystemDominator->Initialize());
			CSubsystemServiceLocator::SetSubsystemDominator(m_SubsystemDominator);
		}

#ifndef RENDER_TEST
		{
			m_RenderingSubsystem = new CRenderingSubsystem();
			CHECK_RESULT_BREAK(m_RenderingSubsystem);

			FRenderingSubsystemInitializer initializer = {};
			initializer.BarrierType = Glue::EResourceBarrierType::BARRIER_TYPE_TRANSITION;
			initializer.CommandListType = COMMAND_LIST_TYPE_DIRECT;
			initializer.CommandQueueFlag = COMMAND_QUEUE_FLAG_NONE;
			initializer.CommandQueuePriority = COMMAND_QUEUE_PRIORITY_NORMAL;
			initializer.ViewWidth = _Initializer->ViewportWidth;
			initializer.ViewHeight = _Initializer->ViewportHeight;
			initializer.WindowHandle = _Initializer->WindowHandle;
			initializer.QueueReserveNum = 200;
			initializer.RTVFormat = FORMAT_R8G8B8A8_UNORM;
			initializer.RTVDimension = RTV_DIMENSION_TEXTURE2D;
			initializer.NearZ = 0.01f;
			initializer.FarZ = 10000.0f;
			initializer.FovAngle = DirectX::XM_PIDIV2;


			CHECK_RESULT_BREAK(m_RenderingSubsystem->Initialize(&initializer));

			m_SubsystemDominator->SetRenderingSubsystem(m_RenderingSubsystem);
		}

		auto m_MainDevice = m_RenderingSubsystem->GetProcessorEdit()->GetDeviceEdit();
#endif

		{
			m_ResourceSubsystem = new CResourceManagementSubsystem();
			CHECK_RESULT_BREAK(m_ResourceSubsystem);

			CResourceManagementSubsystem::FInitializer initializer = {};
			initializer.ResourceTypeNum = EResourceManagementType::RESOURCE_TYPE_NUM;

			CHECK_RESULT_BREAK(m_ResourceSubsystem->Initialize(initializer));

			
			// Texture
			{
				CHECK_RESULT_BREAK(m_ResourceSubsystem->SetupManager<CTextureResourceManager>(EResourceManagementType::RESOURCE_TYPE_TEXTURE));
			}

			// Mesh
			{
				CHECK_RESULT_BREAK(m_ResourceSubsystem->SetupManager<CMeshDataManager>(EResourceManagementType::RESOURCE_TYPE_MESH));
			}

			// Shader
			{
				CHECK_RESULT_BREAK(m_ResourceSubsystem->SetupManager<CShaderManager>(EResourceManagementType::RESOURCE_TYPE_SHADER));
			}

			// Material
			{
				CHECK_RESULT_BREAK(m_ResourceSubsystem->SetupManager<CMaterialManager>(EResourceManagementType::RESOURCE_TYPE_MATERIAL));
			}

			// PSO
			{
				CHECK_RESULT_BREAK(m_ResourceSubsystem->SetupManager<CPipelineStateObjectManager>(EResourceManagementType::RESOURCE_TYPE_PSO));
			}

			m_ResSystemInterface = new CResourceSubsystemImpl();
			CHECK_RESULT_BREAK(m_ResSystemInterface);
			auto ptr = PCast<CResourceSubsystemImpl*>(m_ResSystemInterface);
			
			ptr->SetSubsystem(m_ResourceSubsystem);


			m_SubsystemDominator->SetResourceSubsystem(ptr);

			CHECK_RESULT_BREAK(ptr->CreateDefaultTextureResource(m_MainDevice));

			// DefaultPSO Create
			{
				// Pmd
				{
					FShaderNameBlock nameBlock = {};
					nameBlock.VS.File = "resource/cso/PmdVertexShader.cso";
					nameBlock.VS.Function = "main";
					nameBlock.PS.File = "resource/cso/PmdPixelShader.cso";
					nameBlock.PS.Function = "main";

					CHECK_RESULT_BREAK(ptr->CreatePmdPipelineStateObject(m_MainDevice, nameBlock));
				}
			}
		}

		{
			m_UpdaterSubsystem = new CUpdateProcessorSubsystem();
			CHECK_RESULT_BREAK(m_UpdaterSubsystem);
			
			CHECK_RESULT_BREAK(m_UpdaterSubsystem->Initialize());

			m_SubsystemDominator->SetUpdaterSubsystem(m_UpdaterSubsystem);
		}
		

	HRESULT result = S_OK;

	ID3D12Device* m_Device = m_MainDevice->GetDevice();
	IDXGIFactory6* m_GIFactory = m_MainDevice->GetGIFactory();
	//ID3D12CommandQueue* m_CmdQueue = m_CommandQueue->GetCommandQueue();

	{
		// テストメッシュ

		// Sample
		const vector<XMFLOAT3> sampleMesh =
		{
			{-0.4f,-0.7f,0.0f} ,//左下
			{-0.4f,0.7f,0.0f} ,//左上
			{0.4f,-0.7f,0.0f} ,//右下
			{0.4f,0.7f,0.0f} ,//右上
		};

		const vector<u16> sampleIndices = { 0,1,2, 2,1,3 };

		// Squad
		const vector<XMFLOAT3> square =
		{
			{-0.5f,0.5f,0.0f}, // 左上
			{0.5f,0.5f,0.0f}, //  右上
			{0.5f,-0.5f,0.0f}, // 右下
			{-0.5f,-0.5f,0.0f}, // 左下
		};

		const vector<u16>  squareIndices =
		{
			0,1,2,
			2,3,0,
		};

		struct PositionAndUV
		{
			XMFLOAT3 Position = {};
			XMFLOAT2 UV = {};
		};

#define MESH_TYPE PositionAndUV
#define MESH_VECTOR vector<MESH_TYPE>
#define MESH_PTR MESH_TYPE*

#define TEST_MESH_DECLARATION(x) MESH_VECTOR testMesh = x;

		const vector<PositionAndUV> uvSquare =
		{
			{{-0.5f,0.5f,0.0f},{0.f,0.f}}, // 左上
			{{0.5f,0.5f,0.0f},{1.f,0.f}}, //  右上
			{{0.5f,-0.5f,0.0f},{1.f,1.f}}, // 右下
			{{-0.5f,-0.5f,0.0f},{0.f,1.f}}, // 左下
		};

		//vector<XMFLOAT3> testMesh = sampleMesh;
		TEST_MESH_DECLARATION(uvSquare);
		//vector<XMFLOAT3> testMesh = square;
		vector<u16> testIndicies = squareIndices;
		//vector<u16> testIndicies = sampleIndices;

		for (auto& element : testMesh)
		{
			float scaleRate = 5.f;
			element.Position.x *= scaleRate;
			element.Position.y *= scaleRate;
			element.Position.z *= scaleRate;

		};

		u32 vertexSize = sizeof(MESH_TYPE);

		u32 meshSize = vertexSize * SCast<u32>(testMesh.size());
		u32 indexSize = sizeof(u16) * SCast<u32>(squareIndices.size());

		// 頂点バッファ
		D3D12_HEAP_PROPERTIES heapProperty = {};
		heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC vbDesc = {};
		vbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		vbDesc.Width = meshSize;
		vbDesc.Height = 1;
		vbDesc.DepthOrArraySize = 1;
		vbDesc.MipLevels = 1;
		vbDesc.Format = DXGI_FORMAT_UNKNOWN;
		vbDesc.SampleDesc.Count = 1;
		vbDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		vbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		//result = m_Device->CreateCommittedResource(
		//	&heapProperty,
		//	D3D12_HEAP_FLAG_NONE,
		//	&vbDesc,
		//	D3D12_RESOURCE_STATE_GENERIC_READ,
		//	nullptr,
		//	IID_PPV_ARGS(&m_VertexBuffer));
		//if (FAILED(result)) { return false; }

		D3D_ERROR_CHECK_TEMP(m_Device->CreateCommittedResource(
			&heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&vbDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_VertexBuffer)));

		//XMFLOAT3* vertexMap = nullptr;
		MESH_PTR vertexMap = nullptr;
		result = m_VertexBuffer->Map(0, nullptr, (void**)(&vertexMap));

		std::copy(begin(testMesh), end(testMesh), vertexMap);

		m_VertexBuffer->Unmap(0, nullptr);

		m_VertexBufferView = new D3D12_VERTEX_BUFFER_VIEW();
		if (m_VertexBufferView == nullptr) { return false; }

		m_VertexBufferView->BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
		m_VertexBufferView->SizeInBytes = meshSize;
		m_VertexBufferView->StrideInBytes = vertexSize;

#undef MESH_PTR
#undef MESH_VECTO
#undef MESH_TYPE

		// インデックスバッファ
		D3D12_RESOURCE_DESC ibDesc = {};
		ibDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		ibDesc.Width = indexSize;
		ibDesc.Height = 1;
		ibDesc.DepthOrArraySize = 1;
		ibDesc.MipLevels = 1;
		ibDesc.Format = DXGI_FORMAT_UNKNOWN;
		ibDesc.SampleDesc.Count = 1;
		ibDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		ibDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		//result = m_Device->CreateCommittedResource(
		//	&heapProperty,
		//	D3D12_HEAP_FLAG_NONE,
		//	&ibDesc,
		//	D3D12_RESOURCE_STATE_GENERIC_READ,
		//	nullptr,
		//	IID_PPV_ARGS(&m_IndexBuffer));
		//if (FAILED(result)) { return false; }
		D3D_ERROR_CHECK_TEMP(m_Device->CreateCommittedResource(
			&heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&ibDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_IndexBuffer)));

		u16* indexMap = nullptr;

		m_IndexBuffer->Map(0, nullptr, (void**)&indexMap);

		std::copy(begin(testIndicies), end(testIndicies), indexMap);

		m_IndexBuffer->Unmap(0, nullptr);

		m_IndexBufferView = new D3D12_INDEX_BUFFER_VIEW();
		if (m_IndexBufferView == nullptr) { return false; }

		m_IndexBufferView->BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
		m_IndexBufferView->Format = DXGI_FORMAT_R16_UINT;
		m_IndexBufferView->SizeInBytes = indexSize;
	}

	{
		m_VertexShader = new CVertexShader();
		CHECK_RESULT_BREAK(m_VertexShader);

		//CVertexShader::FInitializer Initializer;
		FVertexShaderInitializer Initializer = {};
		Initializer.Device = m_MainDevice->GetDevice();

		{
			CFileLoader loader = {};
			loader.FileLoad("resource/cso/BasicVertexShader.cso");
			Initializer.Data = loader.GetData();
			Initializer.Size = loader.GetSize();

			string fileAndFuncName = StringUtility::CombineText("resource/cso/BasicVertexShader.cso", "main",'*');
			Initializer.Name = CHash160(fileAndFuncName);
			Initializer.Type = EShaderType::SHADER_TYPE_VERTEX;
#if 0
			{
				CHECK_RESULT_BREAK(m_VertexShader->Initialize(&Initializer));
			}
#else
			{
				FResourceHandle handle = {};
				handle = CSubsystemServiceLocator::GetResourceSubsystem()->GetShaderResourceManager()->SearchCreateResource(&Initializer);

				m_VertexShader = PCast<const CVertexShader*>(CSubsystemServiceLocator::GetResourceSubsystem()->GetShaderResourceManager()->SearchResource(handle));
			}
#endif
			loader.ResetData();
		}

	}

	{
		m_PixelShader = new CPixelShader();
		CHECK_RESULT_BREAK(m_PixelShader);

		//CPixelShader::FInitializer Initializer;
		FPixelShaderInitializer Initializer = {};

		{
			CFileLoader loader = {};
			loader.FileLoad("resource/cso/BasicPixelShader.cso");
			Initializer.Data = loader.GetData();
			Initializer.Size = loader.GetSize();

			string fileAndFuncName = StringUtility::CombineText("resource/cso/BasicPixelShader.cso", "main", '*');
			Initializer.Name = CHash160(fileAndFuncName);

			Initializer.Type = EShaderType::SHADER_TYPE_PIXEL;

#if 0
			CHECK_RESULT_BREAK(m_PixelShader->Initialize(&Initializer));
#else

			{
				FResourceHandle handle = {};
				handle = CSubsystemServiceLocator::GetResourceSubsystem()->GetShaderResourceManager()->SearchCreateResource(&Initializer);

				m_PixelShader = PCast<const CPixelShader*>(CSubsystemServiceLocator::GetResourceSubsystem()->GetShaderResourceManager()->SearchResource(handle));
			}
#endif

			loader.ResetData();
		}

	}

	{
		// ディスクリプタヒープ
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		descHeapDesc.NodeMask = 0;
		//descHeapDesc.NumDescriptors = 1;
		descHeapDesc.NumDescriptors = 2;
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		D3D_ERROR_CHECK_TEMP(m_Device->CreateDescriptorHeap(
			&descHeapDesc,
			IID_PPV_ARGS(&m_DescHeap)
		));
	}


	{
		//　テクスチャデータの作成
		struct TexRGBA
		{
			uint8 R = 0;
			uint8 G = 0;
			uint8 B = 0;
			uint8 A = 0;
		};

		constexpr uint8 COLOR_RANGE = 255;

		const uint32 pixelWidth = 256;
		const uint32 pixelHeight = 256;
		const uint32 pixelSize = pixelWidth * pixelHeight;
		vector<TexRGBA> textureData(pixelSize);

		CRandomizer& random = CRandomizer::GetInstance();

		for (uint32 i = 0; i < pixelSize; ++i)
		{
			auto& data = textureData[i];
			data.R = SCast<uint8>(random.RandomRangeInteger<uint16>(0, COLOR_RANGE));
			data.G = SCast<uint8>(random.RandomRangeInteger<uint16>(0, COLOR_RANGE));
			data.B = SCast<uint8>(random.RandomRangeInteger<uint16>(0, COLOR_RANGE));
			data.A = COLOR_RANGE;
		}

		const char* fileName = "resource/image/IMG_2036.JPG";

		CFileLoader loader = {};
		loader.FileLoad(fileName);


		CTexture imageTexture = {};
		CTexture::FInitializer textureInitializer = {};
		textureInitializer.FileNameHash = CHash160(fileName);
		textureInitializer.Data = loader.GetData();
		textureInitializer.DataSize = loader.GetSize();

		CHECK_RESULT_BREAK(imageTexture.Initialize(textureInitializer));

#define TEXTURE_RESOURCE_TEST 
#ifndef TEXTURE_RESOURCE_TEST
		// テクスチャバッファ
		D3D12_HEAP_PROPERTIES heapProp = {};
		heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
		heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
		heapProp.CreationNodeMask = 0;
		heapProp.VisibleNodeMask = 0;

		D3D12_RESOURCE_DESC resDesc = {};
		resDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//resDesc.Width = pixelWidth;
		//resDesc.Height = pixelHeight;
		resDesc.Width = SCast<uint32>(imageTexture.GetMetaData()->width);
		resDesc.Height = SCast<uint32>(imageTexture.GetMetaData()->height);
		//resDesc.DepthOrArraySize = 1;
		resDesc.DepthOrArraySize = SCast<uint32>(imageTexture.GetMetaData()->arraySize);
		resDesc.SampleDesc.Count = 1;
		resDesc.SampleDesc.Quality = 0;
		//resDesc.MipLevels = 1;
		resDesc.MipLevels = SCast<uint32>(imageTexture.GetMetaData()->mipLevels);
		//resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resDesc.Dimension = SCast<D3D12_RESOURCE_DIMENSION>(imageTexture.GetMetaData()->dimension);
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		ID3D12Resource* textureBuffer = nullptr;

		D3D_ERROR_CHECK_TEMP(m_Device->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			nullptr,
			IID_PPV_ARGS(&textureBuffer)));

		D3D_ERROR_CHECK_TEMP(textureBuffer->WriteToSubresource(
			0,
			nullptr,
			//textureData.data(),
			imageTexture.GetImage()->pixels,
			//SCast<uint32>(sizeof(TexRGBA) * pixelWidth),
			SCast<uint32>(imageTexture.GetImage()->rowPitch),
			//SCast<uint32>(sizeof(TexRGBA) * textureData.size())
			SCast<uint32>(imageTexture.GetImage()->slicePitch)
		));
#endif

		CTextureResource textureResource = {};

		//CTextureResource::FInitializer texInit = {};
		FTextureInitializer texInit = {};
		texInit.Device = m_Device;
		texInit.Name = CHash160(fileName);
		texInit.Width = SCast<uint32>(imageTexture.GetMetaData()->width);
		texInit.Height = SCast<uint32>(imageTexture.GetMetaData()->height);
		texInit.DepthOrArraySize = SCast<uint32>(imageTexture.GetMetaData()->arraySize);
		texInit.MipLevels = SCast<uint32>(imageTexture.GetMetaData()->mipLevels);
		texInit.Format = SCast<Glue::EDataFormat>(imageTexture.GetMetaData()->format);
		texInit.Dimension = SCast<Glue::EResourceDimension>(imageTexture.GetMetaData()->dimension);

		texInit.SourceData = imageTexture.GetImage()->pixels;
		texInit.RowPitch = SCast<uint32>(imageTexture.GetImage()->rowPitch);
		texInit.SlicePitch = SCast<uint32>(imageTexture.GetImage()->slicePitch);

		D3D_ERROR_CHECK_TEMP(textureResource.Initialize(&texInit));


		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		//srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.Format = imageTexture.GetMetaData()->format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

#ifndef TEXTURE_RESOURCE_TEST
		m_Device->CreateShaderResourceView(
			textureBuffer,
			&srvDesc,
			m_DescHeap->GetCPUDescriptorHandleForHeapStart()
		);
#else
		m_Device->CreateShaderResourceView(
			textureResource.GetResource(),
			&srvDesc,
			m_DescHeap->GetCPUDescriptorHandleForHeapStart()
		);
#endif

		imageTexture.Finalize();

	}


	{
		// 定数バッファ

		D3D12_HEAP_PROPERTIES heapProp = {};
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProp.CreationNodeMask = 1;
		heapProp.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC resDesc = {};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Alignment = 0;
		resDesc.Width = SCast<uint32>((sizeof(SceneMatricesData) + 0xff) & ~0xff);
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.Format = DXGI_FORMAT_UNKNOWN;
		resDesc.SampleDesc.Count = 1;
		resDesc.SampleDesc.Quality = 0;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		

		D3D_ERROR_CHECK_TEMP(m_Device->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBuffer)));

		//m_SceneMatricesData = new XMMATRIX();
		m_SceneMatricesData = new SceneMatricesData();
		CHECK_RESULT_BREAK(m_SceneMatricesData);
		m_SceneMatricesData->World = XMMatrixIdentity();
		m_SceneMatricesData->ViewProjection = XMMatrixIdentity();
		//XMMATRIX* mapMatrix;
		result = m_ConstantBuffer->Map(0, nullptr, (void**)&m_SceneMatricesData);
		//*mapMatrix = matrix;
		//m_ConstantBuffer->Unmap(0,nullptr);


		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = SCast<uint32>(m_ConstantBuffer->GetDesc().Width);


		D3D12_CPU_DESCRIPTOR_HANDLE heapHandle = m_DescHeap->GetCPUDescriptorHandleForHeapStart();
		heapHandle.ptr += m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		m_Device->CreateConstantBufferView(
			&cbvDesc,
			heapHandle);


	}


	{
		// pmdファイル読み込み

		// シグネチャ
		constexpr uint8 signatureSize = 3;
		char signature[signatureSize] = {};

		constexpr uint16 modelNameSize = 20;
		constexpr uint16 commentSize = 256;

		// pmdヘッダー
		struct PMDHeader
		{
			float Version = 0.f;
			char ModelName[modelNameSize] = {};
			char comment[commentSize] = {};
		}; // この手前に3バイトのシグネチャがある

		// pmdに格納されている頂点数
		uint32& vertexNum = PmdVertexNum;

		// pmd頂点情報
		struct PMDVertex
		{
			Vector3 Pos = {};
			Vector3 Normal = {};
			Vector2 UV = {};
			uint16 BoneNo[2] = {};
			uint8 BoneWeight = {};
			uint8 EdgeFlag = {};
			uint16 padding = {};
		};

		constexpr uint32 pmdVertexSize = 38;
		constexpr uint32 realPmdVertexSize = sizeof(PMDVertex);


		
		constexpr uint8 TexFilePathLength = 20;

		// pmdマテリアル情報
		struct PMDMaterial
		{
			Vector3 Diffuse = {};
			float Alpha = 0.f;
			float Specularity = 0.f;
			Vector3 SpecularColor = {};
			Vector3 AmbientColor = {};
			uint8 ToonIndex = {};
			uint8 EdgeFlag = {};

			//padding
			uint16 padding = {};

			uint32 IndiciesNum = 0;

			char TexFilePath[TexFilePathLength] = {};
		};

		constexpr uint32 pmdMaterialSize = sizeof(PMDMaterial);


		PMDHeader pmdHeader = {};

		FILE* fp = nullptr;
		errno_t result = {};
		//const char* modelName = "resource/mmd/UserFile/Model/初音ミク.pmd";
		//const char* modelName = "resource/mmd/UserFile/Model/初音ミクVer2.pmd";
		const char* modelName = "resource/mmd/UserFile/Model/初音ミクmetal.pmd";
		//const char* modelName = "resource/mmd/UserFile/Model/巡音ルカ.pmd";
		Hash160 modelNameHash = CHash160(modelName);

		const char* toonMapDirectory = "resource/mmd/Data/";

		result = fopen_s(&fp, modelName, "rb");

		//char error[256] = {};
		//strerror_s(error,result);
		//PRINT("%s\n", error);

		// 先頭3バイトのシグネチャ読み込み
		fread_s(signature, signatureSize * sizeof(char), signatureSize, 1, fp);
		// ヘッダー情報の読み込み
		fread_s(&pmdHeader, sizeof(pmdHeader), sizeof(pmdHeader), 1, fp);
		// 頂点数の読み込み
		fread_s(&vertexNum, sizeof(vertexNum), sizeof(vertexNum), 1, fp);

		PRINT("VertexNum %u\n", vertexNum);

		// 頂点情報バッファの用意
		//vector<uint8> pmdVertices = {};
		vector<PMDVertex> pmdVertices = {};
		//uint32 pmdVerticesBufferSize = vertexNum * pmdVertexSize;
		uint32 pmdVerticesBufferSize = vertexNum;
		pmdVertices.resize(pmdVerticesBufferSize);
		uint32 verticesByteSize = vertexNum * realPmdVertexSize;

		// 頂点情報の読み込み
		//size_t size = 0;
		//size = fread_s(pmdVertices.data(), pmdVertices.size(), sizeof(uint8), pmdVerticesBufferSize, fp);

		for (auto& element : pmdVertices)
		{
			fread_s(&element, realPmdVertexSize, pmdVertexSize, 1, fp);
		}

		// インデックスの数を取得
		uint32& indexNum = PmdIndexNum;
		fread_s(&indexNum, sizeof(indexNum), sizeof(indexNum), 1, fp);

		PRINT("IndexNum %u\n", indexNum);

		// インデックス情報バッファの用意
		vector<uint16> pmdIndicies = {};
		pmdIndicies.resize(indexNum);
		uint32 indexSize = sizeof(pmdIndicies[0]);
		uint32 indiciesByteSize = indexNum * indexSize;

		fread_s(pmdIndicies.data(), indiciesByteSize, indexSize, indexNum, fp);

		// テクスチャパスをルートフォルダからのものに変換する
		function <string(const string&, const char*)> GetTexturePathFromModel= 
			[](const string& modelPath, const char* texturePath)
		{
			if (strlen(texturePath) == 0) { return string(); }
			int32 pathIndex1 = SCast<uint32>(modelPath.rfind('\\'));
			int32 pathIndex2 = SCast<uint32>(modelPath.rfind('/'));
			int32 index = max(pathIndex1, pathIndex2);

			string folderPath = modelPath.substr(0, index + 1);

			return folderPath + texturePath;
		};

		// テクスチャファイル名から拡張子を取得する
		function <string(const string&)> GetExtension = [](const string& _path)
		{
			int32 index = SCast<int32>(_path.rfind('.'));

			return _path.substr(index + 1, _path.length() - index - 1);
		};

		// テクスチャパスをセパレータ文字で分離する
		function <pair<string, string>(const string&)> SplitFileName = [](const string& _path, const char splitter = '*')
		{
			int32 index = SCast<int32>(_path.find(splitter));
			pair<string, string> ret;
			ret.first = _path.substr(0, index);
			ret.second = _path.substr(index + 1, _path.length() - index - 1);

			return ret;
		};

		// マテリアル数の読み込み
		uint32 materialNum = 0;
		fread_s(&materialNum, sizeof(materialNum), sizeof(materialNum), 1, fp);

		// マテリアル情報バッファの用意
		vector<MaterialData>& pmdMaterials = m_MaterialData;
		pmdMaterials.resize(materialNum);

		for (auto& element : pmdMaterials)
		{
#define MATERIAL_PARAM_READ(param1,param2) \
	fread_s(&element.param1, sizeof(element.param1), sizeof(PMDMaterial::param2), 1, fp)\

#define MATERIAL_SOLO_PARAM_READ(param) MATERIAL_PARAM_READ(param,param)
#define MATERIAL_GPU_PARAM_READ(param) MATERIAL_PARAM_READ(gpuData.param,param)
#define MATERIAL_ADD_PARAM_READ(param) MATERIAL_PARAM_READ(addData.param,param)

			//fread_s(&element.Diffuse, sizeof(element.Diffuse), sizeof(element.Diffuse), 1, fp);
			//PMDMaterial material = {};
			// ファイルからの読み込み順のため順番確定

			//fread_s(&element.gpuData.Diffuse, sizeof(element.gpuData.Diffuse), sizeof(PMDMaterial::Diffuse), 1, fp);
			//fread_s(&element.gpuData.Alpha, sizeof(element.gpuData.Alpha), sizeof(PMDMaterial::Alpha), 1, fp);
			//fread_s(&element.gpuData.Specularity, sizeof(element.gpuData.Specularity), sizeof(PMDMaterial::Specularity), 1, fp);
			//fread_s(&element.gpuData.SpecularColor, sizeof(element.gpuData.SpecularColor), sizeof(PMDMaterial::SpecularColor), 1, fp);
			//fread_s(&element.gpuData.AmbientColor, sizeof(element.gpuData.AmbientColor), sizeof(PMDMaterial::AmbientColor), 1, fp);
			//fread_s(&element.addData.ToonIndex, sizeof(element.addData.ToonIndex), sizeof(PMDMaterial::ToonIndex), 1, fp);
			//fread_s(&element.addData.EdgeFlag, sizeof(element.addData.EdgeFlag), sizeof(PMDMaterial::EdgeFlag), 1, fp);
			//fread_s(&element.IndiciesNum, sizeof(element.IndiciesNum), sizeof(PMDMaterial::IndiciesNum), 1, fp);

			MATERIAL_GPU_PARAM_READ(Diffuse);
			MATERIAL_GPU_PARAM_READ(Alpha);
			MATERIAL_GPU_PARAM_READ(Specularity);
			MATERIAL_GPU_PARAM_READ(SpecularColor);
			MATERIAL_GPU_PARAM_READ(AmbientColor);
			MATERIAL_ADD_PARAM_READ(ToonIndex);
			MATERIAL_ADD_PARAM_READ(EdgeFlag);
			MATERIAL_SOLO_PARAM_READ(IndiciesNum);

			char texFilePath[TexFilePathLength] = {};
			fread_s(texFilePath, sizeof(texFilePath[0]) * TexFilePathLength, sizeof(texFilePath[0]), TexFilePathLength, fp);

			//string texName = {};
			//texName.resize(TexFilePathLength);
			//texName.assign(texFilePath);

			//constexpr char splitter = '*';
			//if (count(texName.begin(), texName.end(), splitter) > 0)
			//{
			//	auto namepair = SplitFileName(texName);
			//	auto extension = GetExtension(namepair.first);
			//	if (extension == "sph" || extension == "spa")
			//	{
			//		texName = namepair.second;
			//	}
			//	else
			//	{
			//		texName = namepair.first;
			//	}
			//}

			//string texFolderFilePath = GetTexturePathFromModel(RHash160(modelNameHash), texFilePath);
			//string texFolderFilePath = GetTexturePathFromModel(RHash160(modelNameHash), texName.c_str());
			

			element.addData.TexPathHash = CHash160(texFilePath);

		}
#undef MATERIAL_PARAM_READ



		fclose(fp);



		// 頂点バッファ
		//CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_HEAP_PROPERTIES heapProperty = {};
		//heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
		//heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		//heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		//heapProperty.CreationNodeMask = 1;
		//heapProperty.VisibleNodeMask = 1;
		heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

		//CD3DX12_RESOURCE_DESC::Buffer(pmdVertices.size());
		D3D12_RESOURCE_DESC vbDesc = {};
		//vbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		//vbDesc.Width = pmdVertices.size();
		//vbDesc.Height = 1;
		//vbDesc.DepthOrArraySize = 1;
		//vbDesc.MipLevels = 1;
		//vbDesc.Format = DXGI_FORMAT_UNKNOWN;
		//vbDesc.SampleDesc.Count = 1;
		//vbDesc.SampleDesc.Quality = 0;
		//vbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		//vbDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		vbDesc = CD3DX12_RESOURCE_DESC::Buffer(verticesByteSize);


		D3D_ERROR_CHECK_TEMP(m_Device->CreateCommittedResource(
			&heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&vbDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_PmdVertexBuffer)));


		//uint8* pmdVertexMap = nullptr;
		PMDVertex* pmdVertexMap = nullptr;
		HRESULT hResult = m_PmdVertexBuffer->Map(0, nullptr, (void**)(&pmdVertexMap));
		std::copy(begin(pmdVertices), end(pmdVertices), pmdVertexMap);
		m_PmdVertexBuffer->Unmap(0, nullptr);

		m_PmdVertexBufferView = new D3D12_VERTEX_BUFFER_VIEW();
		if (m_PmdVertexBufferView == nullptr) { return false; }

		m_PmdVertexBufferView->BufferLocation = m_PmdVertexBuffer->GetGPUVirtualAddress();
		m_PmdVertexBufferView->SizeInBytes = verticesByteSize;
		//m_PmdVertexBufferView->StrideInBytes = pmdVertexSize;
		m_PmdVertexBufferView->StrideInBytes = realPmdVertexSize;


		// インデックスバッファ
		D3D12_RESOURCE_DESC ibDesc = {};
		//ibDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		//ibDesc.Width = indexSize;
		//ibDesc.Height = 1;
		//ibDesc.DepthOrArraySize = 1;
		//ibDesc.MipLevels = 1;
		//ibDesc.Format = DXGI_FORMAT_UNKNOWN;
		//ibDesc.SampleDesc.Count = 1;
		//ibDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		//ibDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		ibDesc = CD3DX12_RESOURCE_DESC::Buffer(indiciesByteSize);

		D3D_ERROR_CHECK_TEMP(m_Device->CreateCommittedResource(
			&heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&ibDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_PmdIndexBuffer)));

		uint16* indexMap = nullptr;

		m_PmdIndexBuffer->Map(0, nullptr, (void**)&indexMap);

		std::copy(begin(pmdIndicies), end(pmdIndicies), indexMap);

		m_PmdIndexBuffer->Unmap(0, nullptr);

		m_PmdIndexBufferView = new D3D12_INDEX_BUFFER_VIEW();
		if (m_PmdIndexBufferView == nullptr) { return false; }

		m_PmdIndexBufferView->BufferLocation = m_PmdIndexBuffer->GetGPUVirtualAddress();
		m_PmdIndexBufferView->Format = DXGI_FORMAT_R16_UINT;
		m_PmdIndexBufferView->SizeInBytes = indiciesByteSize;

		{
			m_PmdVertexShader = new CVertexShader();
			CHECK_RESULT_BREAK(m_PmdVertexShader);

			//CVertexShader::FInitializer Initializer;
			FVertexShaderInitializer Initializer = {};
			Initializer.Device = m_MainDevice->GetDevice();

			{
				CFileLoader loader = {};
				loader.FileLoad("resource/shader/PmdVertexShader.hlsl");
				Initializer.Data = loader.GetData();
				Initializer.Size = loader.GetSize();

				string fileAndFuncName = StringUtility::CombineText("resource/shader/PmdVertexShader.hlsl", "main", '*');
				Initializer.Name = CHash160(fileAndFuncName);

				CHECK_RESULT_BREAK(m_PmdVertexShader->Initialize(&Initializer));

				loader.ResetData();
			}

		}

		{
			m_PmdPixelShader = new CPixelShader();
			CHECK_RESULT_BREAK(m_PmdPixelShader);

			//CPixelShader::FInitializer Initializer;
			FPixelShaderInitializer Initializer = {};

			{
				CFileLoader loader = {};
				loader.FileLoad("resource/shader/PmdPixelShader.hlsl");
				Initializer.Data = loader.GetData();
				Initializer.Size = loader.GetSize();

				string fileAndFuncName = StringUtility::CombineText("resource/shader/PmdPixelShader.hlsl", "main", '*');
				Initializer.Name = CHash160(fileAndFuncName);

				CHECK_RESULT_BREAK(m_PmdPixelShader->Initialize(&Initializer));

				loader.ResetData();
			}
		}

		{
			// マテリアルバッファの作成
			uint32 materialBufferSize = sizeof(MaterialForHlsl);
			materialBufferSize = (materialBufferSize + 0xff) & ~0xff;

			D3D12_HEAP_PROPERTIES matHeapProp = {};
			matHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

			D3D12_RESOURCE_DESC matResDesc = {};
			matResDesc = CD3DX12_RESOURCE_DESC::Buffer(materialBufferSize * materialNum);//256バイトアラインなので、イメージはしやすいがアライメントのパディングですんごいメモリが余る


			D3D_ERROR_CHECK_TEMP(m_Device->CreateCommittedResource(
				&matHeapProp,
				D3D12_HEAP_FLAG_NONE,
				&matResDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&m_MaterialBuffer)
			));

			uint8* mapMaterial = nullptr;
			m_MaterialBuffer->Map(0, nullptr, (void**)&mapMaterial);

			for (auto& data : pmdMaterials)
			{
				//*RCast<MaterialForHlsl*>(mapMaterial) = data.gpuData;
				memcpy_s(mapMaterial, materialBufferSize, &data.gpuData, sizeof(pmdMaterials[0]));

				mapMaterial += materialBufferSize;
			}

			m_MaterialBuffer->Unmap(0, nullptr);

			// テクスチャをシェーダーリソース化
			vector<ID3D12Resource*> materialResource = {};
			//vector<CTexture> materialTexture = {};
			//materialTexture.resize(materialNum);
			materialResource.resize(materialNum);
			vector<ID3D12Resource*> sphResource = {};
			sphResource.resize(materialNum);
			vector<ID3D12Resource*> spaResource = {};
			spaResource.resize(materialNum);
			vector<ID3D12Resource*> toonResource = {};
			toonResource.resize(materialNum);


			D3D12_HEAP_PROPERTIES texHeapProp = {};
			texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
			texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
			texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
			texHeapProp.CreationNodeMask = 0;
			texHeapProp.VisibleNodeMask = 0;
			
			// 白テクスチャリソース
			ID3D12Resource* whiteResource = nullptr;
			//黒テクスチャリソース
			ID3D12Resource* blackResource = nullptr;
			{
				// テクスチャが無いときのために、乗算用白色テクスチャを用意する
				// >> 確かに面倒だが、PixelShader的には一番厄介なif文が回避できる明暗
				D3D12_RESOURCE_DESC resDesc = {};

				resDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				resDesc.Width = 4;
				resDesc.Height = 4;
				resDesc.DepthOrArraySize = 1;
				resDesc.SampleDesc.Count = 1;
				resDesc.SampleDesc.Quality = 0;
				resDesc.MipLevels = 1;
				resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
				resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
				resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;


				// Width * Height * Color(R8G8B8A8)
				{
					// 白テクスチャ
					D3D_ERROR_CHECK_TEMP(m_Device->CreateCommittedResource(
						&texHeapProp,
						D3D12_HEAP_FLAG_NONE,
						&resDesc,
						D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
						nullptr,
						IID_PPV_ARGS(&whiteResource)
					));

					vector<uint8> whiteData(4 * 4 * 4);
					std::fill(whiteData.begin(), whiteData.end(), 0xff);

					D3D_ERROR_CHECK_TEMP(whiteResource->WriteToSubresource(
						0,
						nullptr,
						whiteData.data(),
						4 * 4,
						SCast<uint32>(whiteData.size())
					));
				}

				// Width * Height * Color(R8G8B8A8)
				{
					// 黒テクスチャ
					D3D_ERROR_CHECK_TEMP(m_Device->CreateCommittedResource(
						&texHeapProp,
						D3D12_HEAP_FLAG_NONE,
						&resDesc,
						D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
						nullptr,
						IID_PPV_ARGS(&blackResource)
					));

					vector <uint8> blackData(4 * 4 * 4);
					std::fill(blackData.begin(), blackData.end(), 0x00);

					D3D_ERROR_CHECK_TEMP(blackResource->WriteToSubresource(
						0,
						nullptr,
						blackData.data(),
						4 * 4,
						SCast<uint32>(blackData.size())
					));
				}
			}

			// デフォルトトゥーングラデーションテクスチャ
			ID3D12Resource* grayGradationResource = nullptr;
			{
				D3D12_RESOURCE_DESC resDesc = {};

				resDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				resDesc.Width = 4;
				resDesc.Height = 256;
				resDesc.DepthOrArraySize = 1;
				resDesc.SampleDesc.Count = 1;
				resDesc.SampleDesc.Quality = 0;
				resDesc.MipLevels = 1;
				resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
				resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
				resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

				{
					D3D_ERROR_CHECK_TEMP(m_Device->CreateCommittedResource(
						&texHeapProp,
						D3D12_HEAP_FLAG_NONE,
						&resDesc,
						D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
						nullptr,
						IID_PPV_ARGS(&grayGradationResource)
					));

					vector<uint32> grayGradationData(4 * 256);
					//fill(grayGradationData.begin(), grayGradationData.end(), 0xff);
					uint32 color = 0xff;
					for (auto itr = grayGradationData.begin(); itr != grayGradationData.end(); itr += 4)
					{
						auto col = (0xff << 24) | RGB(color, color, color);
						std::fill(itr, itr + 4, col);
						--color;
					}

					D3D_ERROR_CHECK_TEMP(grayGradationResource->WriteToSubresource(
						0,
						nullptr,
						grayGradationData.data(),
						SCast<uint32>(4 * sizeof(uint32)),
						SCast<uint32>(grayGradationData.size() * sizeof(uint32))
					));
				}
			}

			// Diffuseの色が淡すぎる気がする？ >> 全体的に白に寄っている気がする
			for (uint32 index = 0; index < materialNum; ++index)
			{
				string texPath = RHash160(m_MaterialData[index].addData.TexPathHash);
				string textureName = {};
				string sphName = {};
				string spaName = {};
				string modelname = RHash160(modelNameHash);

				constexpr char splitter = '*';
				if (count(texPath.begin(), texPath.end(), splitter) > 0)
				{
					auto namepair = SplitFileName(texPath);
					auto extensionFirst = GetExtension(namepair.first);
					//if (extension == "sph" || extension == "spa")
					//{
					//	textureName = namepair.second;
					//	sphName = namepair.first;
					//}
					//else
					//{
					//	textureName = namepair.first;
					//	sphName = namepair.second;
					//}
					if (extensionFirst == "sph" || extensionFirst == "SPH")
					{
						textureName = namepair.second;
						sphName = namepair.first;
					}
					else if (extensionFirst == "spa" || extensionFirst == "SPA")
					{
						textureName = namepair.second;
						spaName = namepair.first;
					}
					else
					{
						textureName = namepair.first;
						auto extensionSecond = GetExtension(namepair.second);
						if (extensionSecond == "sph" || extensionSecond == "SPH")
						{
							sphName = namepair.second;
						}
						else if (extensionSecond == "spa" || extensionSecond == "SPA")
						{
							spaName = namepair.second;
						}
					}
				}
				else
				{
					auto extension = GetExtension(texPath);
					if (extension == "sph" || extension == "SPH")
					{
						sphName = texPath;
					}
					else if (extension == "spa" || extension == "SPA")
					{
						spaName = texPath;
					}
					else
					{
						textureName = texPath;
					}
				}

				function<bool(const CTexture& _texture)> CheckTextureIsNull =
					[](const CTexture& _texture)
				{
					return _texture.GetMetaData() == nullptr || _texture.GetImage() == nullptr;
				};

				
				CTexture::FInitializer initializer = {};


				// 通常テクスチャ
				{
					string texFolderFilePath = GetTexturePathFromModel(modelName, textureName.c_str());
					initializer.FileNameHash = CHash160(texFolderFilePath);

					CFileLoader loader = {};
					loader.FileLoad(texFolderFilePath.c_str());

					initializer.Data = loader.GetData();
					initializer.DataSize = loader.GetSize();

					CTexture texture = {};
					texture.Initialize(initializer);

					loader.ResetData();

					ID3D12Resource*& resPtr = materialResource[index];

					//if (texture.GetName() == Hash160() || texture.GetMetaData() == nullptr || texture.GetImage() == nullptr)
					if (CheckTextureIsNull(texture))
					{
						const CTextureResource* texPtr = PCast<const CTextureResource*>(CSubsystemServiceLocator::GetResourceSubsystem()->GetTextureResourceManager()->SearchResource(DefaultResource::WhiteTextureResource));
						resPtr = texPtr->GetResource();
						//resPtr = whiteResource;
					}
					else
					{
						D3D12_RESOURCE_DESC resDesc = {};
						resDesc.Format = texture.GetMetaData()->format;
						resDesc.Width = SCast<uint32>(texture.GetMetaData()->width);
						resDesc.Height = SCast<uint32>(texture.GetMetaData()->height);
						resDesc.DepthOrArraySize = SCast<uint32>(texture.GetMetaData()->arraySize);
						resDesc.SampleDesc.Count = 1;
						resDesc.SampleDesc.Quality = 0;
						resDesc.MipLevels = SCast<uint32>(texture.GetMetaData()->mipLevels);
						resDesc.Dimension = SCast<D3D12_RESOURCE_DIMENSION>(texture.GetMetaData()->dimension);
						resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
						resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

						D3D_ERROR_CHECK_TEMP(m_Device->CreateCommittedResource(
							&texHeapProp,
							D3D12_HEAP_FLAG_NONE,
							&resDesc,
							D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
							nullptr,
							IID_PPV_ARGS(&resPtr)
						));

						D3D_ERROR_CHECK_TEMP(resPtr->WriteToSubresource(
							0,
							nullptr,
							texture.GetImage()->pixels,
							SCast<uint32>(texture.GetImage()->rowPitch),
							SCast<uint32>(texture.GetImage()->slicePitch)
						));
					}

					texture.Finalize();
				}


				// スフィアマップ
				{
					CTexture sph = {};
					string sphFolderFilePath = GetTexturePathFromModel(modelName, sphName.c_str());

					CFileLoader loader = {};
					loader.FileLoad(sphFolderFilePath.c_str());

					initializer.FileNameHash = CHash160(sphFolderFilePath);
					initializer.Data = loader.GetData();
					initializer.DataSize = loader.GetSize();
					sph.Initialize(initializer);

					loader.ResetData();

					ID3D12Resource*& sphResPtr = sphResource[index];

					if (CheckTextureIsNull(sph))
					{
						const CTextureResource* texPtr = PCast<const CTextureResource*>(CSubsystemServiceLocator::GetResourceSubsystem()->GetTextureResourceManager()->SearchResource(DefaultResource::WhiteTextureResource));
						sphResPtr = texPtr->GetResource();
					}
					else
					{
						D3D12_RESOURCE_DESC resDesc = {};
						resDesc.Format = sph.GetMetaData()->format;
						resDesc.Width = SCast<uint32>(sph.GetMetaData()->width);
						resDesc.Height = SCast<uint32>(sph.GetMetaData()->height);
						resDesc.DepthOrArraySize = SCast<uint32>(sph.GetMetaData()->arraySize);
						resDesc.SampleDesc.Count = 1;
						resDesc.SampleDesc.Quality = 0;
						resDesc.MipLevels = SCast<uint32>(sph.GetMetaData()->mipLevels);
						resDesc.Dimension = SCast<D3D12_RESOURCE_DIMENSION>(sph.GetMetaData()->dimension);
						resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
						resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

						D3D_ERROR_CHECK_TEMP(m_Device->CreateCommittedResource(
							&texHeapProp,
							D3D12_HEAP_FLAG_NONE,
							&resDesc,
							D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
							nullptr,
							IID_PPV_ARGS(&sphResPtr)
						));

						D3D_ERROR_CHECK_TEMP(sphResPtr->WriteToSubresource(
							0,
							nullptr,
							sph.GetImage()->pixels,
							SCast<uint32>(sph.GetImage()->rowPitch),
							SCast<uint32>(sph.GetImage()->slicePitch)
						));
					}

					sph.Finalize();
				}


				// 加算スフィアマップ
				{
					CTexture spa = {};
					string spaFolderFilePath = GetTexturePathFromModel(modelName, spaName.c_str());

					CFileLoader loader = {};
					loader.FileLoad(spaFolderFilePath.c_str());

					initializer.FileNameHash = CHash160(spaFolderFilePath);
					initializer.Data = loader.GetData();
					initializer.DataSize = loader.GetSize();

					loader.ResetData();

					spa.Initialize(initializer);

					ID3D12Resource*& spaResPtr = spaResource[index];

					if (CheckTextureIsNull(spa))
					{
						const CTextureResource* texPtr = PCast<const CTextureResource*>(CSubsystemServiceLocator::GetResourceSubsystem()->GetTextureResourceManager()->SearchResource(DefaultResource::BlackTextureResource));
						spaResPtr = texPtr->GetResource();
					}
					else
					{
						D3D12_RESOURCE_DESC resDesc = {};
						resDesc.Format = spa.GetMetaData()->format;
						resDesc.Width = SCast<uint32>(spa.GetMetaData()->width);
						resDesc.Height = SCast<uint32>(spa.GetMetaData()->height);
						resDesc.DepthOrArraySize = SCast<uint32>(spa.GetMetaData()->arraySize);
						resDesc.SampleDesc.Count = 1;
						resDesc.SampleDesc.Quality = 0;
						resDesc.MipLevels = SCast<uint32>(spa.GetMetaData()->mipLevels);
						resDesc.Dimension = SCast<D3D12_RESOURCE_DIMENSION>(spa.GetMetaData()->dimension);
						resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
						resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

						D3D_ERROR_CHECK_TEMP(m_Device->CreateCommittedResource(
							&texHeapProp,
							D3D12_HEAP_FLAG_NONE,
							&resDesc,
							D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
							nullptr,
							IID_PPV_ARGS(&spaResPtr)
						));

						D3D_ERROR_CHECK_TEMP(spaResPtr->WriteToSubresource(
							0,
							nullptr,
							spa.GetImage()->pixels,
							SCast<uint32>(spa.GetImage()->rowPitch),
							SCast<uint32>(spa.GetImage()->slicePitch)
						));
					}

					spa.Finalize();
				}

				// トゥーンマップ
				{
					CTexture toon = {};
					
					constexpr uint8 toonTexNameLength = 16;
					char toonTexName[toonTexNameLength] = {};

					sprintf_s(toonTexName, "toon%02d.bmp", SCast<uint8>(m_MaterialData[index].addData.ToonIndex + 1));

					string toonFolderFilePath(toonMapDirectory);
					toonFolderFilePath += toonTexName;

					CFileLoader loader = {};
					loader.FileLoad(toonFolderFilePath.c_str());

					initializer.FileNameHash = CHash160(toonFolderFilePath);
					initializer.Data = loader.GetData();
					initializer.DataSize = loader.GetSize();
					toon.Initialize(initializer);

					loader.ResetData();

					ID3D12Resource*& toonResPtr = toonResource[index];

					if (CheckTextureIsNull(toon))
					{
						const CTextureResource* texPtr = PCast<const CTextureResource*>(CSubsystemServiceLocator::GetResourceSubsystem()->GetTextureResourceManager()->SearchResource(DefaultResource::GrayGradationTextureResource));
						toonResPtr = texPtr->GetResource();
						//toonResPtr = grayGradationResource;
					}
					else
					{
						D3D12_RESOURCE_DESC resDesc = {};
						resDesc.Format = toon.GetMetaData()->format;
						resDesc.Width = SCast<uint32>(toon.GetMetaData()->width);
						resDesc.Height = SCast<uint32>(toon.GetMetaData()->height);
						resDesc.DepthOrArraySize = SCast<uint32>(toon.GetMetaData()->arraySize);
						resDesc.SampleDesc.Count = 1;
						resDesc.SampleDesc.Quality = 0;
						resDesc.MipLevels = SCast<uint32>(toon.GetMetaData()->mipLevels);
						resDesc.Dimension = SCast<D3D12_RESOURCE_DIMENSION>(toon.GetMetaData()->dimension);
						resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
						resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

						D3D_ERROR_CHECK_TEMP(m_Device->CreateCommittedResource(
							&texHeapProp,
							D3D12_HEAP_FLAG_NONE,
							&resDesc,
							D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
							nullptr,
							IID_PPV_ARGS(&toonResPtr)
						));

						D3D_ERROR_CHECK_TEMP(toonResPtr->WriteToSubresource(
							0,
							nullptr,
							toon.GetImage()->pixels,
							SCast<uint32>(toon.GetImage()->rowPitch),
							SCast<uint32>(toon.GetImage()->slicePitch)
						));
					}

					toon.Finalize();

				}

			}

			// ディスクリプタヒープの作成
			D3D12_DESCRIPTOR_HEAP_DESC matDescHeapDesc = {};
			matDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			matDescHeapDesc.NodeMask = 0;
			matDescHeapDesc.NumDescriptors = SCast<uint32>(pmdMaterials.size() + materialResource.size() + sphResource.size() + spaResource.size() + toonResource.size());
			matDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

			D3D_ERROR_CHECK_TEMP(m_Device->CreateDescriptorHeap(
				&matDescHeapDesc,
				IID_PPV_ARGS(&m_MaterialDescHeap)
			));

			// ビューの作成
			D3D12_CONSTANT_BUFFER_VIEW_DESC matCBVDesc = {};
			matCBVDesc.SizeInBytes = materialBufferSize;

			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;

			matCBVDesc.BufferLocation = m_MaterialBuffer->GetGPUVirtualAddress();
			D3D12_CPU_DESCRIPTOR_HANDLE matDescheapHandle = m_MaterialDescHeap->GetCPUDescriptorHandleForHeapStart();

			for (uint32 index = 0; index < materialNum; ++index)
			{
				// コンスタントバッファ
				m_Device->CreateConstantBufferView(
					&matCBVDesc,
					matDescheapHandle
				);

				uint32 increment = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
				matDescheapHandle.ptr += increment;

				// シェーダーリソースビュー（通常テクスチャ）
				{
					auto& resPtr = materialResource[index];
					if (resPtr != nullptr)
					{
						srvDesc.Format = resPtr->GetDesc().Format;
					}

					m_Device->CreateShaderResourceView(
						resPtr,
						&srvDesc,
						matDescheapHandle
					);

					matDescheapHandle.ptr += increment;
				}

				// シェーダーリソースビュー (sph)
				{
					auto& sphResPtr = sphResource[index];
					if (sphResPtr != nullptr)
					{
						srvDesc.Format = sphResPtr->GetDesc().Format;
					}

					m_Device->CreateShaderResourceView(
						sphResPtr,
						&srvDesc,
						matDescheapHandle
					);

					matDescheapHandle.ptr += increment;
				}

				// シェーダーリソースビュー (spa)
				{
					auto& spaResPtr = spaResource[index];
					if (spaResPtr != nullptr)
					{
						srvDesc.Format = spaResPtr->GetDesc().Format;
					}

					m_Device->CreateShaderResourceView(
						spaResPtr,
						&srvDesc,
						matDescheapHandle
					);

					matDescheapHandle.ptr += increment;
				}

				// シェーダーリソースビュー (toon)
				{
					auto& toonResPtr = toonResource[index];
					if (toonResPtr != nullptr)
					{
						srvDesc.Format = toonResPtr->GetDesc().Format;
					}

					m_Device->CreateShaderResourceView(
						toonResPtr,
						&srvDesc,
						matDescheapHandle
					);

					matDescheapHandle.ptr += increment;

				}

				matCBVDesc.BufferLocation += materialBufferSize;
			}

		}

	}


	{
		m_RootSignature = new CRootSignature();
		CHECK_RESULT_BREAK(m_RootSignature);

		//D3D12_DESCRIPTOR_RANGE descTableRange = {};
		//descTableRange.NumDescriptors = 1;
		//descTableRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		//descTableRange.BaseShaderRegister = 0;
		//descTableRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		// ディスクリプタ―テーブル << シェーダーで扱うレジスタの設定
		D3D12_DESCRIPTOR_RANGE descTableRange[2] = {};
		descTableRange[0].NumDescriptors = 1;
		descTableRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descTableRange[0].BaseShaderRegister = 0;
		descTableRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		descTableRange[1].NumDescriptors = 1;
		descTableRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		descTableRange[1].BaseShaderRegister = 0;
		descTableRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;



		//D3D12_ROOT_PARAMETER rootParam = {};
		//rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		//rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		//rootParam.DescriptorTable.pDescriptorRanges = descTableRange;
		//rootParam.DescriptorTable.NumDescriptorRanges = 1;

		D3D12_ROOT_PARAMETER rootParam[2] = {};
		rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParam[0].DescriptorTable.pDescriptorRanges = &descTableRange[0];
		rootParam[0].DescriptorTable.NumDescriptorRanges = 1;

		rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		rootParam[1].DescriptorTable.pDescriptorRanges = &descTableRange[1];
		rootParam[1].DescriptorTable.NumDescriptorRanges = 1;


		D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		//samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;


		CRootSignature::FInitializer Initializer;
		Initializer.Device = m_MainDevice->GetDevice();
		Initializer.Flag = Glue::ERootSignatureFlag::ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		Initializer.pParameters = rootParam;
		Initializer.NumParameters = 2;
		Initializer.pStaticSamplers = &samplerDesc;
		Initializer.NumStaticSamplers = 1;


		CHECK_RESULT_BREAK(m_RootSignature->Initialize(Initializer));
	}


	{
		m_Pipeline = new CGraphicsPipeline();
		CHECK_RESULT_BREAK(m_Pipeline);

		CHECK_RESULT_BREAK(m_Pipeline->Initialize());

		m_Pipeline->SetRootSignature(m_RootSignature);
		m_Pipeline->SetVertexShader(m_VertexShader);
		m_Pipeline->SetPixelShader(m_PixelShader);

		D3D12_GRAPHICS_PIPELINE_STATE_DESC& m_PipelineDesc = *m_Pipeline->GetPipelineDescEdit();

		m_PipelineDesc.BlendState.AlphaToCoverageEnable = true;
		m_PipelineDesc.BlendState.IndependentBlendEnable = false;// こいつがfalseならRenderTargetの設定は最初の1つだけでOKになる Independent => 個別に


		D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
		renderTargetBlendDesc.BlendEnable = false;
		renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		renderTargetBlendDesc.LogicOpEnable = false;

		m_PipelineDesc.BlendState.RenderTarget[0] = renderTargetBlendDesc;


		m_PipelineDesc.RasterizerState.MultisampleEnable = false;
		m_PipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		m_PipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		m_PipelineDesc.RasterizerState.DepthClipEnable = true;

		m_PipelineDesc.RasterizerState.FrontCounterClockwise = false;
		m_PipelineDesc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		m_PipelineDesc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		m_PipelineDesc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		m_PipelineDesc.RasterizerState.AntialiasedLineEnable = false;
		m_PipelineDesc.RasterizerState.ForcedSampleCount = 0;
		m_PipelineDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;


		//m_PipelineDesc.DepthStencilState.DepthEnable = false;
		m_PipelineDesc.DepthStencilState.DepthEnable = true;
		m_PipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		m_PipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // 小さい方（近い方）を採用
		m_PipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		m_PipelineDesc.DepthStencilState.StencilEnable = false;

		m_PipelineDesc.NumRenderTargets = 1;
		m_PipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		//m_PipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;


		CHECK_RESULT_BREAK(m_Pipeline->RecreateState(m_MainDevice));
	}

	{
		{
			m_PmdRootSignature = new CRootSignature();
			CHECK_RESULT_BREAK(m_PmdRootSignature);

			//D3D12_DESCRIPTOR_RANGE descTableRange = {};
			//descTableRange.NumDescriptors = 1;
			//descTableRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
			//descTableRange.BaseShaderRegister = 0;
			//descTableRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

			// ディスクリプタ―テーブル << シェーダーで扱うレジスタの設定
			D3D12_DESCRIPTOR_RANGE descTableRange[3] = {};
			descTableRange[0].NumDescriptors = 1;
			descTableRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
			descTableRange[0].BaseShaderRegister = 0;
			descTableRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

			descTableRange[1].NumDescriptors = 1;
			descTableRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
			descTableRange[1].BaseShaderRegister = 1;
			descTableRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

			// テクスチャ用テーブル
			descTableRange[2].NumDescriptors = 4; //(普通のテクスチャとsphとspaとtoon)
			descTableRange[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
			descTableRange[2].BaseShaderRegister = 0;
			descTableRange[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

			//D3D12_ROOT_PARAMETER rootParam = {};
			//rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			//rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			//rootParam.DescriptorTable.pDescriptorRanges = descTableRange;
			//rootParam.DescriptorTable.NumDescriptorRanges = 1;

			D3D12_ROOT_PARAMETER rootParam[2] = {};
			rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
			rootParam[0].DescriptorTable.pDescriptorRanges = &descTableRange[0];
			rootParam[0].DescriptorTable.NumDescriptorRanges = 1;

			rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParam[1].DescriptorTable.pDescriptorRanges = &descTableRange[1];
			rootParam[1].DescriptorTable.NumDescriptorRanges = 2;


			//D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
			//samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			//samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			//samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			//samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			////samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
			//samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			//samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
			//samplerDesc.MinLOD = 0.0f;
			//samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			//samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;

			D3D12_STATIC_SAMPLER_DESC samplerDesc[2] = {};
			samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			samplerDesc[0].BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			samplerDesc[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc[0].MaxLOD = D3D12_FLOAT32_MAX;
			samplerDesc[0].MinLOD = 0.0f;
			samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			samplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			samplerDesc[0].ShaderRegister = 0;

			samplerDesc[1] = samplerDesc[0];
			samplerDesc[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			samplerDesc[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			samplerDesc[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			samplerDesc[1].ShaderRegister = 1;


			CRootSignature::FInitializer Initializer;
			Initializer.Device = m_MainDevice->GetDevice();
			Initializer.Flag = Glue::ERootSignatureFlag::ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
			//Initializer.pParameters = nullptr;
			//Initializer.NumParameters = 0;
			//Initializer.pStaticSamplers = nullptr;
			//Initializer.NumStaticSamplers = 0;
			Initializer.pParameters = rootParam;
			Initializer.NumParameters = 2;
			Initializer.pStaticSamplers = samplerDesc;
			Initializer.NumStaticSamplers = 2;



			CHECK_RESULT_BREAK(m_PmdRootSignature->Initialize(Initializer));
		}

		// Pmd用パイプライン
		m_PmdPipeline = new CGraphicsPipeline();
		CHECK_RESULT_BREAK(m_PmdPipeline);

		CHECK_RESULT_BREAK(m_PmdPipeline->Initialize());

		m_PmdPipeline->SetRootSignature(m_PmdRootSignature);
		m_PmdPipeline->SetVertexShader(m_PmdVertexShader);
		m_PmdPipeline->SetPixelShader(m_PmdPixelShader);

		D3D12_GRAPHICS_PIPELINE_STATE_DESC& m_PmdPipelineDesc = *m_PmdPipeline->GetPipelineDescEdit();

		m_PmdPipelineDesc.BlendState.AlphaToCoverageEnable = true;
		m_PmdPipelineDesc.BlendState.IndependentBlendEnable = false;// こいつがfalseならRenderTargetの設定は最初の1つだけでOKになる Independent => 個別に


		D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
		renderTargetBlendDesc.BlendEnable = false;
		renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		renderTargetBlendDesc.LogicOpEnable = false;

		m_PmdPipelineDesc.BlendState.RenderTarget[0] = renderTargetBlendDesc;


		m_PmdPipelineDesc.RasterizerState.MultisampleEnable = false;
		m_PmdPipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		m_PmdPipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		//m_PmdPipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		m_PmdPipelineDesc.RasterizerState.DepthClipEnable = true;

		m_PmdPipelineDesc.RasterizerState.FrontCounterClockwise = false;
		m_PmdPipelineDesc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		m_PmdPipelineDesc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		m_PmdPipelineDesc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		m_PmdPipelineDesc.RasterizerState.AntialiasedLineEnable = false;
		m_PmdPipelineDesc.RasterizerState.ForcedSampleCount = 0;
		m_PmdPipelineDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;


		//m_PmdPipelineDesc.DepthStencilState.DepthEnable = false;
		m_PmdPipelineDesc.DepthStencilState.DepthEnable = true;
		m_PmdPipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		m_PmdPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // 小さい方（近い方）を採用
		m_PmdPipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		m_PmdPipelineDesc.DepthStencilState.StencilEnable = false;

		m_PmdPipelineDesc.NumRenderTargets = 1;
		m_PmdPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		//m_PmdPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;


		CHECK_RESULT_BREAK(m_PmdPipeline->RecreateState(m_MainDevice));
	}

	{
		// ビュー行列の作成
		float eyeHeight = 16.5f;
		Vector3 eyePos(0.f, eyeHeight, -8.f);
		Vector3 focusPos(0.f,eyeHeight,0.f);
		Vector3 upDir(0.f, 1.f, 0.f);

		FVector eyePosVec = XMLoadFloat3(&eyePos);
		FVector focusPosVec = XMLoadFloat3(&focusPos);
		FVector upDirVec = XMLoadFloat3(&upDir);

		ViewMatrix = XMMatrixLookAtLH(eyePosVec,focusPosVec,upDirVec);


		// プロジェクション行列の作成
		float fovAngle = XM_PIDIV2;
		float aspectRatio = SCast<float>(_Initializer->ViewportWidth) / SCast<float>(_Initializer->ViewportHeight);
		float nearZ = 0.001f;
		float farZ = 1000.f;

		ProjectionMatrix = XMMatrixPerspectiveFovLH(fovAngle, aspectRatio, nearZ, farZ);

		m_SceneMatricesData->EyePosition = eyePos;
		//m_SceneMatricesData->EyePosition = Vector3(0.0f,1.f,1.f);

		m_SceneMatricesData->LightPosition = Vector3(5.f, 20.f, -8.f);
		//m_SceneMatricesData->LightPosition = Vector3(1.f, 0.f, 0.f);
		m_SceneMatricesData->LightColor = Vector3(1.f, 1.f, 1.f);

	}

	//Test::MatrixVectorTestMain();

	Test::FileLoaderTest(m_MainDevice);
	Test::GraphicsPipelineTest(m_MainDevice);
	Test::BindFunctionTest();

#ifndef RENDER_TEST
	Test::RenderSubsystemTest();
#endif

	return true;

	}while (0);


	Finalize();
	return false;
}

void CGameManager::Finalize()
{

	if (m_VertexBufferView != nullptr)
	{
		delete m_VertexBufferView;
	}

	FinalizeObject(m_PmdPipeline);
	FinalizeObject(m_Pipeline);
	FinalizeObject(m_RootSignature);
	//FinalizeObject(m_PixelShader);
	//FinalizeObject(m_VertexShader);
	//FinalizeObject(m_ScissorRect);
	//FinalizeObject(m_Viewport);
	//FinalizeObject(m_Fence);
	//FinalizeObject(m_SwapChain);
	//FinalizeObject(m_CommandQueue);
	//FinalizeObject(m_CommandContext);
	//FinalizeObject(m_MainDevice);

	FinalizeObject(m_UpdaterSubsystem);
	FinalizeObject(m_RenderingSubsystem);
	//FinalizeObject(m_ResSystemInterface);
	Release(m_ResSystemInterface);
	FinalizeObject(m_ResourceSubsystem);
	FinalizeObject(m_SubsystemDominator);
}

void CGameManager::GameMain()
{
	GameUpdate();
	Render();
}

void CGameManager::GameUpdate()
{
#ifndef RENDER_TEST
	/* Render Update */
	m_RenderingSubsystem->ClearRenderGroup();
#endif

	static float rotSpeed = 0.f;

	XMMATRIX pose = XMMatrixIdentity();
	XMMATRIX trans = XMMatrixIdentity();
	XMMATRIX scale = XMMatrixIdentity();
	pose = XMMatrixRotationY((XM_PI / 180.f) * rotSpeed);
	//scale = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	//trans = XMMatrixTranslation(-8.f, -5.f, -5.f);
	//pose = XMMatrixScaling(rotSpeed, rotSpeed, rotSpeed);

	//HRESULT result = m_ConstantBuffer->Map(0, nullptr, (void**)&m_SceneMatricesData);
	//if(result == S_OK)
	//{
	//	*m_SceneMatricesData = pose;
	//	m_ConstantBuffer->Unmap(0, nullptr);
	//}
	//*m_SceneMatricesData = scale * pose * trans * ViewMatrix * ProjectionMatrix;

	//*m_SceneMatricesData = scale * pose * trans * ViewMatrix * ProjectionMatrix;
	m_SceneMatricesData->World = scale * pose * trans;
	m_SceneMatricesData->View = ViewMatrix;
	m_SceneMatricesData->ViewProjection = ViewMatrix * ProjectionMatrix;
	m_SceneMatricesData->WorldViewProjection = m_SceneMatricesData->World * m_SceneMatricesData->ViewProjection;



	rotSpeed += 1.0f;
	if (rotSpeed > 360.f)
	{
		rotSpeed = 0.f;
	}



	// ビュー行列の作成
	float eyeHeight = 16.5f;
	Vector3 eyePos(0.f, eyeHeight, -8.f);
	Vector3 focusPos(0.f, eyeHeight, 0.f);
	Vector3 upDir(0.f, 1.f, 0.f);

	FVector eyePosVec = XMLoadFloat3(&eyePos);
	FVector focusPosVec = XMLoadFloat3(&focusPos);
	FVector upDirVec = XMLoadFloat3(&upDir);

	ViewMatrix = XMMatrixLookAtLH(eyePosVec, focusPosVec, upDirVec);


	// プロジェクション行列の作成
#ifndef RENDER_TEST
	ProjectionMatrix = m_RenderingSubsystem->GetSceneView()->GetProjectionMatrix();;
#endif


	SceneData.World = scale * pose * trans;
	SceneData.View = ViewMatrix;
	SceneData.ViewProjection = ViewMatrix * ProjectionMatrix;
	SceneData.WorldViewProjection = SceneData.World * SceneData.ViewProjection;
	SceneData.EyePosition = eyePos;
	SceneData.LightPosition = Vector3(5.f, 20.f, -8.f);
	SceneData.LightColor = Vector3(1.f, 1.f, 1.f);
	
	Test::m_MaterialData->SetGeometryBuffer(&SceneData);
#ifndef RENDER_TEST
	Test::m_PmdModelComponent->SetSceneData(&SceneData);
#endif
	// 処理時間の制限を行う場所を、AppManagerからここに移動させてもいいかもしれない
	// >> そもそも処理時間を今制限していない問題
	m_UpdaterSubsystem->ProcessorUpdate(0.f);// DeltaTimeを持ってくる

}

void CGameManager::Render()
{
#ifndef RENDER_TEST
	m_RenderingSubsystem->Rendering();
#endif

#ifdef RENDER_TEST
	ID3D12GraphicsCommandList* m_CmdList = m_CommandContext->GetCommandList();

	uint32 bufferIndex = m_SwapChain->GetCurrentBufferIndex();

	auto rtvResource = m_RTV[bufferIndex]->GetResource();
	CBarrier::FTransitionState transitionState = {};
	transitionState.Resource = rtvResource;
	transitionState.Before = Glue::EResourceState::RESOURCE_STATE_PRESENT;
	transitionState.After = Glue::EResourceState::RESOURCE_STATE_RENDER_TARGET;
	m_Barrier->SetTransitionState(transitionState);

	m_CommandContext->Barrier(m_Barrier);


	m_CommandContext->OMSetRenderTarget(m_RTV[bufferIndex], m_DepthStencilView);

	m_CommandContext->SetViewport(m_Viewport);
	m_CommandContext->SetScissorRect(m_ScissorRect);


	m_CommandContext->SetPipelineState(m_Pipeline);
	m_CommandContext->SetRootSignature(m_RootSignature);


	//D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_RtvHeaps->GetCPUDescriptorHandleForHeapStart();
	//rtvHandle.ptr += SCast<ULONG_PTR>(backBufferIndex * m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	//
	//m_CmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

	// レンダーターゲットのクリア
	float clearColor[] = { 1.f,1.f,1.f,1.f };
	m_CommandContext->ClearRenderTargetView(m_RTV[bufferIndex], clearColor);

	// デプスバッファのクリア
	m_CommandContext->ClearDepthStencilView(m_DepthStencilView, 1.f);


	m_CmdList->SetDescriptorHeaps(1, &m_DescHeap);
	auto descHandle = m_DescHeap->GetGPUDescriptorHandleForHeapStart();
	m_CmdList->SetGraphicsRootDescriptorTable(0,descHandle);
	descHandle.ptr += m_MainDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_CmdList->SetGraphicsRootDescriptorTable(1, descHandle);


	m_CmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_CmdList->IASetVertexBuffers(0,1,m_VertexBufferView);
	m_CmdList->IASetIndexBuffer(m_IndexBufferView);
	m_CmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	{
		//m_CommandContext->SetPipelineState(m_PmdPipeline);
		//m_CommandContext->SetRootSignature(m_PmdRootSignature);
		m_CommandContext->SetPipelineState(Test::m_MaterialData->GetGraphicsPipeline());
		m_CommandContext->SetRootSignature(Test::m_MaterialData->GetRootSignature());

		//m_CmdList->IASetVertexBuffers(0, 1, m_PmdVertexBufferView);
		//m_CmdList->IASetIndexBuffer(m_PmdIndexBufferView);

		m_CommandContext->SetVertexBuffer(Test::m_MeshData->GetVertexBuffer());
		m_CommandContext->SetIndexBuffer(Test::m_MeshData->GetIndexBuffer());

		//m_CmdList->SetDescriptorHeaps(1, &m_DescHeap);
		//m_CmdList->SetGraphicsRootDescriptorTable(0, descHandle);


#if 0
		m_CmdList->SetDescriptorHeaps(1, &m_MaterialDescHeap);
		auto matDescHandle = m_MaterialDescHeap->GetGPUDescriptorHandleForHeapStart();
		//m_CmdList->SetGraphicsRootDescriptorTable(1, matDescHandle);
		
		uint32 handleSize = m_MainDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		uint32 indexOffset = 0;
		uint32 i = 0;
		for (const auto& element : m_MaterialData)
		{
			m_CmdList->SetGraphicsRootDescriptorTable(1, matDescHandle);
			m_CmdList->DrawIndexedInstanced(element.IndiciesNum, 1, indexOffset, 0, 0);

			
			matDescHandle.ptr += handleSize;// マテリアルデータの分
			matDescHandle.ptr += handleSize;// マテリアルテクスチャの分
			matDescHandle.ptr += handleSize;// sphテクスチャの分
			matDescHandle.ptr += handleSize;// spaテクスチャの分
			matDescHandle.ptr += handleSize;// toonテクスチャの分
			indexOffset += element.IndiciesNum;
			//++i;
		}
#else

		//ID3D12DescriptorHeap* descHeapArray[] = { Test::m_MaterialData->GetBufferDescriptorHeap(),Test::m_MaterialData->GetDescriptorHeap() };


		// DescriptorTableとRootSignature、
		// そしてDescriptorHeapの関連性の理解が間違っている可能性が大きい
		// >> 原点は最初のポリゴンを出力したのと同じ要領のはず
		// >> 魔導書のSampleも見ながら修復するしかない
#if 0
		m_CmdList->SetDescriptorHeaps(1, Test::m_MaterialData->GetBufferDescriptorHeapPtr());
		//m_CmdList->SetDescriptorHeaps(2, descHeapArray);

		auto bufDescHandle = Test::m_MaterialData->GetBufferDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
		m_CmdList->SetGraphicsRootDescriptorTable(0, bufDescHandle);
#endif

		m_CmdList->SetDescriptorHeaps(1, Test::m_MaterialData->GetDescriptorHeapPtr());

		//m_CmdList->SetGraphicsRootDescriptorTable(0, matDescHandle);

		//matDescHandle.ptr += m_MainDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		auto matDescHandle = Test::m_MaterialData->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
		
		uint32 indexOffset = 0;

		//vector<uint32> indexVec;
		vector<uint32> indexVec{  };

		auto CheckIgnoreIndex = [&](uint32 _Index)
		{
			for (auto& elem : indexVec)
			{
				if (_Index != elem) { continue; }
				return true;
			}
			return false;
		};
		
		for (uint32 index = 0; index < Test::m_MaterialData->GetMaterialNum(); ++index)
		{

			if (!CheckIgnoreIndex(index))
			{

				m_CmdList->SetGraphicsRootDescriptorTable(1, matDescHandle);
				m_CmdList->DrawIndexedInstanced(Test::m_MaterialData->GetDrawIndex(index), 1, indexOffset, 0, 0);
			}

			indexOffset += Test::m_MaterialData->GetDrawIndex(index);
			matDescHandle.ptr += Test::m_MaterialData->GetHeapStride();
		}


#endif

		//m_CmdList->DrawInstanced(PmdVertexNum, 1, 0, 0);
		//m_CmdList->DrawIndexedInstanced(PmdIndexNum, 1, 0, 0, 0);
	}



	//barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	//m_CmdList->ResourceBarrier(1, &barrierDesc);

	transitionState.Before = Glue::EResourceState::RESOURCE_STATE_RENDER_TARGET;
	transitionState.After = Glue::EResourceState::RESOURCE_STATE_PRESENT;
	m_Barrier->SetTransitionState(transitionState);
	m_CommandContext->Barrier(m_Barrier);

	// 命令をクローズ
	m_CommandContext->Close();


	// コマンドリストの実行
	m_CommandQueue->CommandListPush(m_CommandContext);
	m_CommandQueue->Execute();

	// 待ち
	m_CommandQueue->Signal(m_Fence);

	m_Fence->WaitEvent();

	//m_CommandContext->Reset(m_PmdPipeline->GetPipelineState());
	//m_CommandContext->Reset(m_Pipeline);
	m_CommandContext->Reset();

	// フリップ
	m_SwapChain->Present();
#endif

}
