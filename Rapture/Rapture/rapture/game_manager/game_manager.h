#pragma once

#include <vector>
#include <memory>

#include <d3d12.h>

//#include <Atlantis/DirectX12/MainDevice/MainDevice.h>
//#include <Atlantis/DirectX12/Command/CommandContext.h>
//#include <Atlantis/DirectX12/Command/CommandQueue.h>
//#include <Atlantis/DirectX12/SwapChain/SwapChain.h>
//#include <Atlantis/DirectX12/Fence/Fence.h>

ATLANTIS_NAMESPACE_BEGIN

class CDX12MainDevice;
class CDirectXDebug;
class CCommandContext;
class CCommandQueue;
class CSwapChain;
class CFence;
class CRenderTargetView;
class CBarrier;
class CDepthStencilView;

class CViewport;
class CScissorRect;

class CVertexShader;
class CPixelShader;

class CGraphicsPipeline;
class CRootSignature;


ATLANTIS_NAMESPACE_END

BIFROST_NAMESPACE_BEGIN

class CSubsystemDominator;
class CResourceManagementSubsystem;

class IResourceSubsystem;

class CUpdateProcessorSubsystem;
class CRenderingSubsystem;

BIFROST_NAMESPACE_END

/*
*	ゲーム処理を統括するマネージャー
*/
class CGameManager
{
public:

	b8 Initialize(struct FGameManagerInitializer* _Initializer);

	void Finalize();

	void GameMain();

private:

	void GameUpdate();

	void Render();

	ObjectPtr(BIFROST_NAMESPACE::CSubsystemDominator) m_SubsystemDominator = nullptr;
	ObjectPtr(BIFROST_NAMESPACE::CResourceManagementSubsystem) m_ResourceSubsystem = nullptr;

	ObjectPtr(BIFROST_NAMESPACE::IResourceSubsystem) m_ResSystemInterface = nullptr;

	ObjectPtr(BIFROST_NAMESPACE::CUpdateProcessorSubsystem) m_UpdaterSubsystem = nullptr;
	ObjectPtr(BIFROST_NAMESPACE::CRenderingSubsystem) m_RenderingSubsystem = nullptr;

#define RENDER_TEST
#ifdef RENDER_TEST
	ATLANTIS_NAMESPACE::CDX12MainDevice* m_MainDevice = nullptr;
	ATLANTIS_NAMESPACE::CDirectXDebug* m_Debug = nullptr;
	ATLANTIS_NAMESPACE::CCommandContext* m_CommandContext = nullptr;
	ATLANTIS_NAMESPACE::CCommandQueue* m_CommandQueue = nullptr;
	ATLANTIS_NAMESPACE::CSwapChain* m_SwapChain = nullptr;
	ATLANTIS_NAMESPACE::CFence* m_Fence = nullptr;

	ATLANTIS_NAMESPACE::CBarrier* m_Barrier = nullptr;

	ATLANTIS_NAMESPACE::CViewport* m_Viewport = nullptr;
	ATLANTIS_NAMESPACE::CScissorRect* m_ScissorRect = nullptr;
#endif
	const ATLANTIS_NAMESPACE::CVertexShader* m_VertexShader = nullptr;
	const ATLANTIS_NAMESPACE::CPixelShader* m_PixelShader = nullptr;

	ATLANTIS_NAMESPACE::CGraphicsPipeline* m_Pipeline = nullptr;
	ATLANTIS_NAMESPACE::CGraphicsPipeline* m_PmdPipeline = nullptr;
	ATLANTIS_NAMESPACE::CRootSignature* m_RootSignature = nullptr;
	ATLANTIS_NAMESPACE::CRootSignature* m_PmdRootSignature = nullptr;

	std::vector<ATLANTIS_NAMESPACE::CRenderTargetView*> m_RTV = {};

	ATLANTIS_NAMESPACE::CDepthStencilView* m_DepthStencilView = nullptr;

	//// こいつは1つのはず
	//struct ID3D12Device* m_Device = nullptr;

	//// コマンドリストが複数個ある場合はそれぞれ必要
	//struct ID3D12CommandAllocator* m_CmdAllocator = nulchrome://vivaldi-webui/startpage?section=Speed-dials&background-color=#2e2f37lptr;
	//struct ID3D12GraphicsCommandList* m_CmdList = nullptr;
	//struct ID3D12CommandQueue* m_CmdQueue = nullptr;

	//// ディスクリプタヒープはディスクリプタの数存在する。
	//// >>今回の用途でいくと、RTV毎にあっても良さそう。
	//struct ID3D12DescriptorHeap* m_RtvHeaps = nullptr;

	//// フェンスはGPUの処理が完了したかどうかを見ることができる
	//// >>こいつはつまり１描画に１つ対応する形だろうから、
	//// >> 同時に描画しない限りは複数なくても良さそう。
	//struct ID3D12Fence* m_Fence = nullptr;


	// メッシュ単位で持つもの
	struct ID3D12Resource* m_VertexBuffer = nullptr;
	struct D3D12_VERTEX_BUFFER_VIEW* m_VertexBufferView = nullptr;

	struct ID3D12Resource* m_IndexBuffer = nullptr;
	struct D3D12_INDEX_BUFFER_VIEW* m_IndexBufferView = nullptr;

	struct ID3D12Resource* m_ConstantBuffer = nullptr;
	// -メッシュ単位で持つもの

	// メッシュ単位で持つもの
	struct ID3D12Resource* m_PmdVertexBuffer = nullptr;
	struct D3D12_VERTEX_BUFFER_VIEW* m_PmdVertexBufferView = nullptr;

	struct ID3D12Resource* m_PmdIndexBuffer = nullptr;
	struct D3D12_INDEX_BUFFER_VIEW* m_PmdIndexBufferView = nullptr;

	struct ID3D12Resource* m_PmdConstantBuffer = nullptr;

	ATLANTIS_NAMESPACE::CVertexShader* m_PmdVertexShader = nullptr;
	ATLANTIS_NAMESPACE::CPixelShader* m_PmdPixelShader = nullptr;
	// -メッシュ単位で持つもの

	// 深度バッファ
	//struct ID3D12Resource* m_DepthStencilBuffer = nullptr;

	// テクスチャ
	//struct ID3D12Resource* m_TextureBuffer = nullptr;

	// ディスクリプタ―ヒープ
	struct ID3D12DescriptorHeap* m_DescHeap = nullptr;

	// 深度バッファ用ディスクリプタ―ヒープ
	//struct ID3D12DescriptorHeap* m_DepthDescHeap = nullptr;


	// GPU送信用行列ポインタ
	//DirectX::XMMATRIX* m_SceneMatricesData = nullptr;
	struct SceneMatricesData* m_SceneMatricesData = nullptr;

	// マテリアルバッファ
	struct ID3D12Resource* m_MaterialBuffer = nullptr;
	//マテリアル用ディスクリプタヒープ
	struct ID3D12DescriptorHeap* m_MaterialDescHeap = nullptr;


	// グラフィックパイプライン
	//struct D3D12_GRAPHICS_PIPELINE_STATE_DESC* m_PipeLine = nullptr;
	//struct ID3D12PipelineState* m_PipeLineState = nullptr;

	// ルートシグネチャ
	//struct ID3D12RootSignature* m_RootSignature = nullptr;

	//// ビューポート
	//struct D3D12_VIEWPORT* m_Viewport = nullptr;

	// 切り抜き矩形
	//D3D12_RECT m_ScissorRect = {};

	//// GIFactoryはDeviceと同じように他のオブジェクトを生み出すのに使うので
	//// 1つで良さそうだ。
	//struct IDXGIFactory6* m_GIFactory = nullptr;

	//// スワップチェーンは画面の切り替え時に使うものなので、
	//// こいつも1つで良さそう
	//struct IDXGISwapChain4* m_SwapChain = nullptr;


	//// BackBufferはRTVに使っているので、あれとセットだろう
	//std::vector<struct ID3D12Resource*> m_BackBuffers;

	//// フェンスとセット
	//u64 m_FenceValue = 0;

};