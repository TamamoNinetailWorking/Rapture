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

#include <Atlantis/DirectX12/DirectXTex/DirectXTex.h>
#include <Atlantis/DirectX12/DirectXHelper/d3dx12.h>

#include <eden/include/utility/ender_utility.h>

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>

#include <Atlantis/DirectX12/MainDevice/MainDevice.h>
#include <Atlantis/DirectX12/Command/CommandContext.h>
#include <Atlantis/DirectX12/Command/CommandQueue.h>
#include <Atlantis/DirectX12/SwapChain/SwapChain.h>
#include <Atlantis/DirectX12/Fence/Fence.h>

#include <Atlantis/DirectX12/Viewport/Viewport.h>
#include <Atlantis/DirectX12/ScissorRect/ScissorRect.h>
#include <Atlantis/DirectX12/Shader/GraphicsShader.h>

#include <Atlantis/DirectX12/Texture/Texture.h>

#include <Atlantis/DirectX12/GraphicsPipeline/GraphicsPipeline.h>
#include <Atlantis/DirectX12/RootSignature/RootSignature.h>

#include <rapture/Test/Test.h>
#include <eden/include/math/random_utility.h>

using namespace std;
using namespace DirectX;

EDENS_NAMESPACE_USING;
USING_ATLANTIS;

using namespace Glue;

//XMMATRIX* m_SceneMatricesData = nullptr;

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
//uint32 size = sizeof(SceneMatricesData);

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

//#define ARRAYOF(_Array) sizeof(_Array) / sizeof(_Array[0])

#define D3D_ERROR_CHECK_TEMP(result) if(FAILED(result)) { return false; };

b8 CGameManager::Initialize(FGameManagerInitializer * _Initializer)
{
	// DirectX12を初期化 //

	do 
	{
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
	
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		//rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		CSwapChain::FSwapChainInitializer swapchainInitializer;
		swapchainInitializer.Device = m_MainDevice->GetDevice();
		swapchainInitializer.Factory = m_MainDevice->GetGIFactory();
		swapchainInitializer.Queue = m_CommandQueue->GetCommandQueue();
		swapchainInitializer.ViewportWidth = _Initializer->ViewportWidth;
		swapchainInitializer.ViewportHeight = _Initializer->ViewportHeight;
		swapchainInitializer.WindowHandle = _Initializer->WindowHandle;
		swapchainInitializer.BufferCount = 2;
		swapchainInitializer.RtvDesc = &rtvDesc;
		


		if (!m_SwapChain->Initialize(swapchainInitializer))
		{
			break;
		}


		m_Fence = new CFence();
		if (!m_Fence) { break; }

		CFence::FFenceInitializer fenceInitializer;
		fenceInitializer.Device = m_MainDevice->GetDevice();

		if (!m_Fence->Initialize(fenceInitializer))
		{
			break;
		}

		m_Viewport = new CViewport();
		if (!m_Viewport) { break; };

		CViewport::FViewportInitializer viewportInitializer;
		viewportInitializer.ViewportWidth = SCast<float>(_Initializer->ViewportWidth);
		viewportInitializer.VIewportHeight = SCast<float>(_Initializer->ViewportHeight);
		if (!m_Viewport->Initialize(viewportInitializer))
		{
			break;
		}

		{
			m_ScissorRect = new CScissorRect();
			CHECK_RESULT_BREAK(m_ScissorRect);

			CScissorRect::FInitializer initializer = {};
			initializer.RectWidth = _Initializer->ViewportWidth;
			initializer.RectHeight = _Initializer->ViewportHeight;

			CHECK_RESULT_BREAK(m_ScissorRect->Initialize(initializer));
			
		}
		
		


#if 0
#ifdef DEBUG_MODE
	// デバッグレイヤーをONにする
	ID3D12Debug* debugLayer = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer))))
	{
		debugLayer->EnableDebugLayer();

		// デバッグレイヤーを有効化した後はこのインターフェースは不要らしい
		debugLayer->Release();
	}
#endif


	// GIFactoryの生成
	if (FAILED(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&m_GIFactory))))
	{
		//if (FAILED(CreateDXGIFactory2(0, IID_PPV_ARGS(&m_GIFactory))))
		//{
		//	return false;
		//}
		D3D_ERROR_CHECK_TEMP(CreateDXGIFactory2(0, IID_PPV_ARGS(&m_GIFactory)));
	}

	// GIFactoryからアダプター（グラフィックドライバー）の取得
	vector<IDXGIAdapter*> adapters;
	IDXGIAdapter* tempAdapter = nullptr;
	for (u32 i = 0; ; ++i)
	{
		if (m_GIFactory->EnumAdapters(i, &tempAdapter) == DXGI_ERROR_NOT_FOUND)
		{
			break;
		}

		// 刺さっているグラフィックドライバーを配列に入れていく
		adapters.push_back(tempAdapter);
	}

	for (auto& elem : adapters)
	{
		DXGI_ADAPTER_DESC desc = {};
		elem->GetDesc(&desc);

		wstring strDesc = desc.Description;
		if (strDesc.find(L"NVIDIA") != std::string::npos)
		{
			tempAdapter = elem;
			break;
		}
	}

	// フィーチャーレベル列挙
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	// 取得したアダプター情報からデバイスを初期化
	HRESULT result = S_OK;
	D3D_FEATURE_LEVEL featureLevel;
	for (auto& elem : levels)
	{
		result = D3D12CreateDevice(tempAdapter, elem, IID_PPV_ARGS(&m_Device));

		if (result == S_OK)
		{
			// 成功したフィーチャーレベルを保持しておく
			featureLevel = elem;
			break;
		}
	}

	D3D_ERROR_CHECK_TEMP(result);
	//if (FAILED(result)) { return false; }

#else
	HRESULT result = S_OK;
#endif

	ID3D12Device* m_Device = m_MainDevice->GetDevice();
	IDXGIFactory6* m_GIFactory = m_MainDevice->GetGIFactory();
	ID3D12CommandQueue* m_CmdQueue = m_CommandQueue->GetCommandQueue();


#if 0
	// コマンドアロケーターの生成
	//result = m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CmdAllocator));
	//if (FAILED(result)) { return false; }
	D3D_ERROR_CHECK_TEMP(m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CmdAllocator)));

	// コマンドリストの生成
	//result = m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CmdAllocator, nullptr, IID_PPV_ARGS(&m_CmdList));
	//if (FAILED(result)) { return false; }
	D3D_ERROR_CHECK_TEMP(m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CmdAllocator, nullptr, IID_PPV_ARGS(&m_CmdList)));

	// コマンドキューの生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE; // タイムアウト無し
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL; // 優先度設定特に無し
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; // コマンドリストと同様のものを指定

	//result = m_Device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&m_CmdQueue));
	//if (FAILED(result)) { return false; }
	D3D_ERROR_CHECK_TEMP(m_Device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&m_CmdQueue)));
#endif

#if 0
	// スワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
	swapchainDesc.Width = _Initializer->ViewportWidth;
	swapchainDesc.Height = _Initializer->ViewportHeight;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.Stereo = false;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.SampleDesc.Quality = 0;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.BufferCount = 2;
	swapchainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//result = m_GIFactory->CreateSwapChainForHwnd(
	//	m_CmdQueue,
	//	_Initializer->WindowHandle,
	//	&swapchainDesc,
	//	nullptr,
	//	nullptr,
	//	(IDXGISwapChain1**)&m_SwapChain);
	//if (FAILED(result)) { return false; }

	D3D_ERROR_CHECK_TEMP(m_GIFactory->CreateSwapChainForHwnd(
		m_CmdQueue,
		_Initializer->WindowHandle,
		&swapchainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)&m_SwapChain));

	// ディスクリプターヒープ？ DirectX12がやりとりするファイルブロックのようだが・・・
	// >> どうやらスワップチェーンで実際に切り替えるレンダーターゲットビューを設定しているようだ
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットを作成しているようだ
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 2; // 表裏の2つ
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // 特にフラグの指定は無し

	//result = m_Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_RtvHeaps));
	//if (FAILED(result)) { return false; }
	D3D_ERROR_CHECK_TEMP(m_Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_RtvHeaps)));

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	//result = m_SwapChain->GetDesc(&swcDesc);
	//if(FAILED(result)) { return false; }
	D3D_ERROR_CHECK_TEMP(m_SwapChain->GetDesc(&swcDesc));

	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_RtvHeaps->GetCPUDescriptorHandleForHeapStart();

	m_BackBuffers.resize(swcDesc.BufferCount);
	for (u32 i = 0; i < m_BackBuffers.size(); ++i)
	{
		//result = m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_BackBuffers[i]));
		//if (FAILED(result)) { return false; }
		D3D_ERROR_CHECK_TEMP(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_BackBuffers[i])));

		m_Device->CreateRenderTargetView(m_BackBuffers[i], nullptr, handle);
		handle.ptr += m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
#endif

#if 0
	// フェンス？
	// GPUの処理待ちを行うための仕組みの1つらしい
	/*result = m_Device->CreateFence(m_FenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));*/
	D3D_ERROR_CHECK_TEMP(m_Device->CreateFence(m_FenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));
#endif
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

		copy(begin(testMesh), end(testMesh), vertexMap);

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

		copy(begin(testIndicies), end(testIndicies), indexMap);

		m_IndexBuffer->Unmap(0, nullptr);

		m_IndexBufferView = new D3D12_INDEX_BUFFER_VIEW();
		if (m_IndexBufferView == nullptr) { return false; }

		m_IndexBufferView->BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
		m_IndexBufferView->Format = DXGI_FORMAT_R16_UINT;
		m_IndexBufferView->SizeInBytes = indexSize;
	}

#if 0
	// シェーダー // 
	ID3DBlob* errorBlob = nullptr;

	auto CompileFunction = [&](LPCWSTR _FileName, LPCSTR _FuncName, LPCSTR _Target, ID3DBlob*& _Blob)
	{
		result = D3DCompileFromFile(
			_FileName,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			_FuncName,
			_Target,
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&_Blob,
			&errorBlob);

		if (FAILED(result))
		{
			if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
			{
				PRINT("ファイルが存在しません。");
			}
			else
			{
				string errorString;
				errorString.resize(errorBlob->GetBufferSize());

				copy_n(PCast<char*>(errorBlob->GetBufferPointer()), errorBlob->GetBufferSize(), errorString.begin());
				errorString += "\n";

				PRINT(errorString.c_str());
			}
			return S_FALSE;
		}

		return S_OK;

	};

	// VertexShader
	ID3DBlob* vsBlob = nullptr;
	D3D_ERROR_CHECK_TEMP(CompileFunction(L"resource/shader/BasicVertexShader.hlsl", "main", "vs_5_0", vsBlob));

	// PixelShader 
	ID3DBlob* psBlob = nullptr;
	D3D_ERROR_CHECK_TEMP(CompileFunction(L"resource/shader/BasicPixelShader.hlsl", "main", "ps_5_0",psBlob));
#endif

	{
		m_VertexShader = new CVertexShader();
		CHECK_RESULT_BREAK(m_VertexShader);

		CVertexShader::FInitializer Initializer;
		Initializer.Device = m_MainDevice->GetDevice();
		Initializer.FileNameHash = CHash160("resource/shader/BasicVertexShader.hlsl");
		Initializer.FuncNameHash = CHash160("main");

		CHECK_RESULT_BREAK(m_VertexShader->Initialize(&Initializer));

	}

	{
		m_PixelShader = new CPixelShader();
		CHECK_RESULT_BREAK(m_PixelShader);

		CPixelShader::FInitializer Initializer;
		Initializer.FileNameHash = CHash160("resource/shader/BasicPixelShader.hlsl");
		Initializer.FuncNameHash = CHash160("main");

		CHECK_RESULT_BREAK(m_PixelShader->Initialize(&Initializer));
	}

#if 0
	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	};
#endif

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
		// 深度バッファ用のディスクリプタヒープ
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.NumDescriptors = 1;
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

		D3D_ERROR_CHECK_TEMP(m_Device->CreateDescriptorHeap(
			&descHeapDesc,
			IID_PPV_ARGS(&m_DepthDescHeap)
		));

	}

	{
		// デプスバッファの作成
		D3D12_RESOURCE_DESC depthResDesc = {};
		depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		depthResDesc.Width = _Initializer->ViewportWidth;
		depthResDesc.Height = _Initializer->ViewportHeight;
		depthResDesc.DepthOrArraySize = 1;
		depthResDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度用フォーマット
		depthResDesc.SampleDesc.Count = 1;
		depthResDesc.SampleDesc.Quality = 0;
		depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;//デプスステンシルとして使用

		D3D12_HEAP_PROPERTIES depthHeapProp = {};
		depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
		depthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		depthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_CLEAR_VALUE depthClearValue = {};
		depthClearValue.DepthStencil.Depth = 1.f; // 深度値の最大値（一番奥)でクリアする
		depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;


		D3D_ERROR_CHECK_TEMP(m_Device->CreateCommittedResource(
			&depthHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&depthResDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度書き込み用
			&depthClearValue,
			IID_PPV_ARGS(&m_DepthStencilBuffer)
		));


		// デプスステンシルバッファ用のビューを作成
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

		// ビューを作成してディスクリプタヒープに登録
		m_Device->CreateDepthStencilView(
			m_DepthStencilBuffer,
			&dsvDesc,
			m_DepthDescHeap->GetCPUDescriptorHandleForHeapStart()
		);

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

		CTexture imageTexture = {};
		CTexture::FInitializer textureInitializer = {};
		textureInitializer.FileNameHash = CHash160("resource/image/IMG_2036.JPG");
		CHECK_RESULT_BREAK(imageTexture.Initialize(&textureInitializer));

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


		D3D_ERROR_CHECK_TEMP(m_Device->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			nullptr,
			IID_PPV_ARGS(&m_TextureBuffer)));

		D3D_ERROR_CHECK_TEMP(m_TextureBuffer->WriteToSubresource(
			0,
			nullptr,
			//textureData.data(),
			imageTexture.GetImage()->pixels,
			//SCast<uint32>(sizeof(TexRGBA) * pixelWidth),
			SCast<uint32>(imageTexture.GetImage()->rowPitch),
			//SCast<uint32>(sizeof(TexRGBA) * textureData.size())
			SCast<uint32>(imageTexture.GetImage()->slicePitch)
		));

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		//srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.Format = imageTexture.GetMetaData()->format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		m_Device->CreateShaderResourceView(
			m_TextureBuffer,
			&srvDesc,
			m_DescHeap->GetCPUDescriptorHandleForHeapStart()
		);

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
		copy(begin(pmdVertices), end(pmdVertices), pmdVertexMap);
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

		copy(begin(pmdIndicies), end(pmdIndicies), indexMap);

		m_PmdIndexBuffer->Unmap(0, nullptr);

		m_PmdIndexBufferView = new D3D12_INDEX_BUFFER_VIEW();
		if (m_PmdIndexBufferView == nullptr) { return false; }

		m_PmdIndexBufferView->BufferLocation = m_PmdIndexBuffer->GetGPUVirtualAddress();
		m_PmdIndexBufferView->Format = DXGI_FORMAT_R16_UINT;
		m_PmdIndexBufferView->SizeInBytes = indiciesByteSize;

		{
			m_PmdVertexShader = new CVertexShader();
			CHECK_RESULT_BREAK(m_PmdVertexShader);

			CVertexShader::FInitializer Initializer;
			Initializer.Device = m_MainDevice->GetDevice();
			Initializer.FileNameHash = CHash160("resource/shader/PmdVertexShader.hlsl");
			Initializer.FuncNameHash = CHash160("main");

			CHECK_RESULT_BREAK(m_PmdVertexShader->Initialize(&Initializer));

		}

		{
			m_PmdPixelShader = new CPixelShader();
			CHECK_RESULT_BREAK(m_PmdPixelShader);

			CPixelShader::FInitializer Initializer;
			Initializer.FileNameHash = CHash160("resource/shader/PmdPixelShader.hlsl");
			Initializer.FuncNameHash = CHash160("main");

			CHECK_RESULT_BREAK(m_PmdPixelShader->Initialize(&Initializer));
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
					fill(whiteData.begin(), whiteData.end(), 0xff);

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
					fill(blackData.begin(), blackData.end(), 0x00);

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
						fill(itr, itr + 4, col);
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
					return _texture.GetName() == Hash160() || _texture.GetMetaData() == nullptr || _texture.GetImage() == nullptr;
				};

				
				CTexture::FInitializer initializer = {};

				// 通常テクスチャ
				{
					string texFolderFilePath = GetTexturePathFromModel(modelName, textureName.c_str());
					initializer.FileNameHash = CHash160(texFolderFilePath);

					CTexture texture = {};
					texture.Initialize(&initializer);


					ID3D12Resource*& resPtr = materialResource[index];

					//if (texture.GetName() == Hash160() || texture.GetMetaData() == nullptr || texture.GetImage() == nullptr)
					if (CheckTextureIsNull(texture))
					{
						resPtr = whiteResource;
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
					initializer.FileNameHash = CHash160(sphFolderFilePath);
					sph.Initialize(&initializer);

					ID3D12Resource*& sphResPtr = sphResource[index];

					if (CheckTextureIsNull(sph))
					{
						sphResPtr = whiteResource;
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
					initializer.FileNameHash = CHash160(spaFolderFilePath);
					spa.Initialize(&initializer);

					ID3D12Resource*& spaResPtr = spaResource[index];

					if (CheckTextureIsNull(spa))
					{
						spaResPtr = blackResource;
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

					initializer.FileNameHash = CHash160(toonFolderFilePath);
					toon.Initialize(&initializer);

					ID3D12Resource*& toonResPtr = toonResource[index];

					if (CheckTextureIsNull(toon))
					{
						toonResPtr = grayGradationResource;
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

	

#if 0
	// グラフィックパイプライン
	m_PipeLine = new D3D12_GRAPHICS_PIPELINE_STATE_DESC();
	if (m_PipeLine == nullptr) { return false; };

	m_PipeLine->pRootSignature = nullptr;

	m_PipeLine->VS.pShaderBytecode = m_VertexShader->GetShaderBytecod();
	m_PipeLine->VS.BytecodeLength = m_VertexShader->GetBufferSize();

	m_PipeLine->InputLayout.pInputElementDescs = m_VertexShader->GetInputLayout();
	m_PipeLine->InputLayout.NumElements = m_VertexShader->GetInputNum();

	m_PipeLine->PS.pShaderBytecode = m_PixelShader->GetShaderBytecod();
	m_PipeLine->PS.BytecodeLength = m_PixelShader->GetBufferSize();

	m_PipeLine->SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	m_PipeLine->BlendState.AlphaToCoverageEnable = false;
	m_PipeLine->BlendState.IndependentBlendEnable = false;


	D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
	renderTargetBlendDesc.BlendEnable = false;
	renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	renderTargetBlendDesc.LogicOpEnable = false;

	m_PipeLine->BlendState.RenderTarget[0] = renderTargetBlendDesc;


	m_PipeLine->RasterizerState.MultisampleEnable = false;
	m_PipeLine->RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	m_PipeLine->RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	m_PipeLine->RasterizerState.DepthClipEnable = true;

	m_PipeLine->RasterizerState.FrontCounterClockwise = false;
	m_PipeLine->RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	m_PipeLine->RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	m_PipeLine->RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	m_PipeLine->RasterizerState.AntialiasedLineEnable = false;
	m_PipeLine->RasterizerState.ForcedSampleCount = 0;
	m_PipeLine->RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;


	m_PipeLine->DepthStencilState.DepthEnable = false;
	m_PipeLine->DepthStencilState.StencilEnable = false;

	m_PipeLine->IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;

	m_PipeLine->PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	m_PipeLine->NumRenderTargets = 1;
	m_PipeLine->RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	m_PipeLine->SampleDesc.Count = 1;
	m_PipeLine->SampleDesc.Quality = 0;
#endif

#if 0
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* rootSignatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	D3D_ERROR_CHECK_TEMP(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSignatureBlob, &errorBlob));
	
	D3D_ERROR_CHECK_TEMP(m_Device->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)));

	rootSignatureBlob->Release();

	m_PipeLine->pRootSignature = m_RootSignature;



	// パイプラインステート
	//D3D_ERROR_CHECK_TEMP(m_Device->CreateGraphicsPipelineState(m_PipeLine, IID_PPV_ARGS(&m_PipeLineState)));
	result = m_Device->CreateGraphicsPipelineState(m_PipeLine, IID_PPV_ARGS(&m_PipeLineState));
	if (FAILED(result))
	{
		break;
	}
#endif


#if 0
	// ビューポート
	m_Viewport = new D3D12_VIEWPORT();
	if (m_Viewport == nullptr) { return false; }
	m_Viewport->Width = SCast<f32>(_Initializer->ViewportWidth);
	m_Viewport->Height = SCast<f32>(_Initializer->ViewportHeight);
	m_Viewport->TopLeftX = 0;
	m_Viewport->TopLeftY = 0;
	m_Viewport->MaxDepth = 1.f;
	m_Viewport->MinDepth = 0.f;
#endif

#if 0
	// 切り抜き矩形
	m_ScissorRect.top = 0;
	m_ScissorRect.left = 0;
	m_ScissorRect.right = m_ScissorRect.left + _Initializer->ViewportWidth;
	m_ScissorRect.bottom = m_ScissorRect.top + _Initializer->ViewportHeight;
#endif

	{
		// ビュー行列の作成
		float eyeHeight = 17.5f;
		Vector3 eyePos(0.f, eyeHeight, -6.f);
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


	/// <summary>
	///  SHA1 Test
	/// </summary>
	/// <param name="_Initializer"></param>
	/// <returns></returns>
	//Test::SHA1TestMain();

	//Test::MatrixVectorTestMain();

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
	FinalizeObject(m_PixelShader);
	FinalizeObject(m_VertexShader);
	FinalizeObject(m_ScissorRect);
	FinalizeObject(m_Viewport);
	FinalizeObject(m_Fence);
	FinalizeObject(m_SwapChain);
	FinalizeObject(m_CommandQueue);
	FinalizeObject(m_CommandContext);
	FinalizeObject(m_MainDevice);

}

void CGameManager::GameMain()
{
	GameUpdate();
	Render();
}

void CGameManager::GameUpdate()
{

	/* Render Update */

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
	m_SceneMatricesData->ViewProjection = ViewMatrix* ProjectionMatrix;
	m_SceneMatricesData->WorldViewProjection = m_SceneMatricesData->World * m_SceneMatricesData->ViewProjection;



	rotSpeed += 1.0f;
	if (rotSpeed > 360.f)
	{
		rotSpeed = 0.f;
	}                                                                       

}

void CGameManager::Render()
{
	//ID3D12Device* m_Device = m_MainDevice->GetDevice();
	ID3D12GraphicsCommandList* m_CmdList = m_CommandContext->GetCommandList();
	//ID3D12CommandQueue* m_CmdQueue = m_CommandQueue->GetCommandQueue();

#if 0
	u32 backBufferIndex = m_SwapChain->GetCurrentBackBufferIndex();

	D3D12_RESOURCE_BARRIER barrierDesc = {};
	barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrierDesc.Transition.pResource = m_BackBuffers[backBufferIndex];
	barrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	m_CmdList->ResourceBarrier(1, &barrierDesc);
#endif

	m_SwapChain->SetDepthBuffer(m_DepthDescHeap);
	m_SwapChain->Begin(m_MainDevice,m_CommandContext);

	// パイプラインセット
	//m_CmdList->SetPipelineState(m_PipeLineState);

	//m_CmdList->RSSetViewports(1, m_Viewport);
	m_CommandContext->SetViewport(m_Viewport);
	//m_CmdList->RSSetScissorRects(1, &m_ScissorRect);
	m_CommandContext->SetScissorRect(m_ScissorRect);


	m_CommandContext->SetPipelineState(m_Pipeline);
	m_CommandContext->SetRootSignature(m_RootSignature);


	//D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_RtvHeaps->GetCPUDescriptorHandleForHeapStart();
	//rtvHandle.ptr += SCast<ULONG_PTR>(backBufferIndex * m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	//
	//m_CmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

	// レンダーターゲットのクリア
	float clearColor[] = { 1.f,1.f,1.f,1.f };
	m_CmdList->ClearRenderTargetView(m_SwapChain->GetRenderTargetViewHandle(), clearColor, 0, nullptr);

	// デプスバッファのクリア
	auto depthHandle = m_DepthDescHeap->GetCPUDescriptorHandleForHeapStart();
	m_CmdList->ClearDepthStencilView(depthHandle, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);



	//m_CmdList->SetGraphicsRootSignature(m_RootSignature);


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
		m_CommandContext->SetPipelineState(m_PmdPipeline);
		m_CommandContext->SetRootSignature(m_PmdRootSignature);

		m_CmdList->IASetVertexBuffers(0, 1, m_PmdVertexBufferView);
		m_CmdList->IASetIndexBuffer(m_PmdIndexBufferView);

		m_CmdList->SetDescriptorHeaps(1, &m_DescHeap);
		m_CmdList->SetGraphicsRootDescriptorTable(0, descHandle);



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

		//m_CmdList->DrawInstanced(PmdVertexNum, 1, 0, 0);
		//m_CmdList->DrawIndexedInstanced(PmdIndexNum, 1, 0, 0, 0);
	}



	//barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	//m_CmdList->ResourceBarrier(1, &barrierDesc);

	m_SwapChain->End(m_CommandContext);

	// 命令をクローズ
	m_CommandContext->Close();


	// コマンドリストの実行
	m_CommandQueue->CommandListPush(m_CommandContext);
	m_CommandQueue->Execute();

	// 待ち
	m_CommandQueue->Signal(m_Fence);

	m_Fence->WaitEvent();

	//m_CommandContext->Reset(m_PmdPipeline->GetPipelineState());
	m_CommandContext->Reset(m_Pipeline->GetPipelineState());

	// フリップ
	m_SwapChain->Present();


}
