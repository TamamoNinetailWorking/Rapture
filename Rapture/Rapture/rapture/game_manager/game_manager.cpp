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

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

#include <eden/include/utility/ender_utility.h>

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>

#include <Atlantis/DirectX12/MainDevice/MainDevice.h>
#include <Atlantis/DirectX12/Command/CommandContext.h>
#include <Atlantis/DirectX12/Command/CommandQueue.h>

#include <rapture/Test/Test.h>

using namespace std;
using namespace DirectX;

EDENS_NAMESPACE_USING;
USING_ATLANTIS;

//#define ARRAYOF(_Array) sizeof(_Array) / sizeof(_Array[0])

#define D3D_ERROR_CHECK_TEMP(result) if(FAILED(result)) { return false; };

b8 CGameManager::Initialize(FGameManagerInitializer * _Initializer)
{
	// DirectX12を初期化 //

	do 
	{
		m_MainDevice = move(unique_ptr<CDX12MainDevice>(new CDX12MainDevice()));
		if (!m_MainDevice) { break; };

		if (!m_MainDevice->Initialize()){ break;}

		m_CommandContext = move(unique_ptr<CCommandContext>(new CCommandContext()));
		if (!m_CommandContext) { break; };

		CCommandContext::FCommandContextInitializer Initializer;
		Initializer.Device = m_MainDevice->GetDevice();
		Initializer.listType = Glue::COMMAND_LIST_TYPE_DIRECT;
		Initializer.NodeMask = 0;


		if (!m_CommandContext->Initialize(Initializer))
		{
			break;
		}

		m_CommandQueue = move(unique_ptr<CCommandQueue>(new CCommandQueue()));
		if (!m_CommandQueue) { break; };

		CCommandQueue::FCommandQueueInitializer queueInitializer;
		queueInitializer.Device = m_MainDevice->GetDevice();
		queueInitializer.listType = Initializer.listType;
		queueInitializer.QueueFlag = Glue::COMMAND_QUEUE_FLAG_NONE;
		queueInitializer.QueuePriority = Glue::COMMAND_QUEUE_PRIORITY_NORMAL;

		if (!m_CommandQueue->Initialize(queueInitializer))
		{
			break;
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

	// フェンス？
	// GPUの処理待ちを行うための仕組みの1つらしい
	/*result = m_Device->CreateFence(m_FenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));*/
	D3D_ERROR_CHECK_TEMP(m_Device->CreateFence(m_FenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));


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

	vector<XMFLOAT3> testMesh = sampleMesh;
	//vector<XMFLOAT3> testMesh = square;
	//vector<u16> testIndicies = squareIndices;
	vector<u16> testIndicies = sampleIndices;

	u32 meshSize = sizeof(XMFLOAT3) * SCast<u32>(testMesh.size());
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

	XMFLOAT3* vertexMap = nullptr;
	result = m_VertexBuffer->Map(0, nullptr, (void**)(&vertexMap));

	copy(begin(testMesh), end(testMesh), vertexMap);

	m_VertexBuffer->Unmap(0, nullptr);

	m_VertexBufferView = new D3D12_VERTEX_BUFFER_VIEW();
	if (m_VertexBufferView == nullptr) { return false; }

	m_VertexBufferView->BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
	m_VertexBufferView->SizeInBytes = meshSize;
	m_VertexBufferView->StrideInBytes = sizeof(XMFLOAT3);


	// インデックスバッファ
	D3D12_RESOURCE_DESC ibDesc = {};
	ibDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ibDesc.Width = meshSize;
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

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	};


	// グラフィックパイプライン
	m_PipeLine = new D3D12_GRAPHICS_PIPELINE_STATE_DESC();
	if (m_PipeLine == nullptr) { return false; };

	m_PipeLine->pRootSignature = nullptr;

	m_PipeLine->VS.pShaderBytecode = vsBlob->GetBufferPointer();
	m_PipeLine->VS.BytecodeLength = vsBlob->GetBufferSize();

	m_PipeLine->InputLayout.pInputElementDescs = inputLayout;
	m_PipeLine->InputLayout.NumElements = ARRAYOF(inputLayout);

	m_PipeLine->PS.pShaderBytecode = psBlob->GetBufferPointer();
	m_PipeLine->PS.BytecodeLength = psBlob->GetBufferSize();

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


	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* rootSignatureBlob = nullptr;
	D3D_ERROR_CHECK_TEMP(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSignatureBlob, &errorBlob));
	
	D3D_ERROR_CHECK_TEMP(m_Device->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)));

	rootSignatureBlob->Release();

	m_PipeLine->pRootSignature = m_RootSignature;


	// パイプラインステート
	D3D_ERROR_CHECK_TEMP(m_Device->CreateGraphicsPipelineState(m_PipeLine, IID_PPV_ARGS(&m_PipeLineState)));


	// ビューポート
	m_Viewport = new D3D12_VIEWPORT();
	if (m_Viewport == nullptr) { return false; }
	m_Viewport->Width = SCast<f32>(_Initializer->ViewportWidth);
	m_Viewport->Height = SCast<f32>(_Initializer->ViewportHeight);
	m_Viewport->TopLeftX = 0;
	m_Viewport->TopLeftY = 0;
	m_Viewport->MaxDepth = 1.f;
	m_Viewport->MinDepth = 0.f;

	// 切り抜き矩形
	m_ScissorRect.top = 0;
	m_ScissorRect.left = 0;
	m_ScissorRect.right = m_ScissorRect.left + _Initializer->ViewportWidth;
	m_ScissorRect.bottom = m_ScissorRect.top + _Initializer->ViewportHeight;


	/// <summary>
	///  SHA1 Test
	/// </summary>
	/// <param name="_Initializer"></param>
	/// <returns></returns>
	//Test::SHA1TestMain();

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

	m_SwapChain->Release();
	m_SwapChain = nullptr;

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

}

void CGameManager::Render()
{
	ID3D12Device* m_Device = m_MainDevice->GetDevice();
	ID3D12GraphicsCommandList* m_CmdList = m_CommandContext->GetCommandList();
	ID3D12CommandQueue* m_CmdQueue = m_CommandQueue->GetCommandQueue();

	u32 backBufferIndex = m_SwapChain->GetCurrentBackBufferIndex();

	D3D12_RESOURCE_BARRIER barrierDesc = {};
	barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrierDesc.Transition.pResource = m_BackBuffers[backBufferIndex];
	barrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	m_CmdList->ResourceBarrier(1, &barrierDesc);

	// パイプラインセット
	m_CmdList->SetPipelineState(m_PipeLineState);


	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_RtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += SCast<ULONG_PTR>(backBufferIndex * m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	
	m_CmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);


	float clearColor[] = { 0.f,0.f,0.f,1.f };
	m_CmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	m_CmdList->RSSetViewports(1, m_Viewport);
	m_CmdList->RSSetScissorRects(1, &m_ScissorRect);
	
	m_CmdList->SetGraphicsRootSignature(m_RootSignature);

	m_CmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_CmdList->IASetVertexBuffers(0,1,m_VertexBufferView);
	m_CmdList->IASetIndexBuffer(m_IndexBufferView);

	m_CmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);


	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	m_CmdList->ResourceBarrier(1, &barrierDesc);

	// 命令をクローズ
	m_CmdList->Close();


	// コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { m_CmdList };
	m_CmdQueue->ExecuteCommandLists(1, cmdLists);

	// 待ち
	m_CmdQueue->Signal(m_Fence, ++m_FenceValue);

	if (m_Fence->GetCompletedValue() != m_FenceValue)
	{
		auto event = CreateEvent(nullptr, false, false, nullptr);
		m_Fence->SetEventOnCompletion(m_FenceValue, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	m_CommandContext->Reset();

	// フリップ
	m_SwapChain->Present(1, 0);


}
