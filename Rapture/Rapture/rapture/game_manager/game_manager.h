#pragma once

#include <vector>
#include <memory>

#include <d3d12.h>

#include <Atlantis/DirectX12/MainDevice/MainDevice.h>
#include <Atlantis/DirectX12/Command/CommandContext.h>
#include <Atlantis/DirectX12/Command/CommandQueue.h>

ATLANTIS_NAMESPACE_BEGIN

class CDX12MainDevice;
class CCommandContext;
class CCommandQueue;

ATLANTIS_NAMESPACE_END

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


	std::unique_ptr<ATLANTIS_NAMESPACE::CDX12MainDevice> m_MainDevice = nullptr;
	std::unique_ptr<ATLANTIS_NAMESPACE::CCommandContext> m_CommandContext = nullptr;
	std::unique_ptr<ATLANTIS_NAMESPACE::CCommandQueue> m_CommandQueue = nullptr;


	//// こいつは1つのはず
	//struct ID3D12Device* m_Device = nullptr;

	//// コマンドリストが複数個ある場合はそれぞれ必要
	//struct ID3D12CommandAllocator* m_CmdAllocator = nullptr;
	//struct ID3D12GraphicsCommandList* m_CmdList = nullptr;
	//struct ID3D12CommandQueue* m_CmdQueue = nullptr;

	// ディスクリプタヒープはディスクリプタの数存在する。
	// >>今回の用途でいくと、RTV毎にあっても良さそう。
	struct ID3D12DescriptorHeap* m_RtvHeaps = nullptr;

	// フェンスはGPUの処理が完了したかどうかを見ることができる
	// >>こいつはつまり１描画に１つ対応する形だろうから、
	// >> 同時に描画しない限りは複数なくても良さそう。
	struct ID3D12Fence* m_Fence = nullptr;


	// メッシュ単位で持つもの
	struct ID3D12Resource* m_VertexBuffer = nullptr;
	struct D3D12_VERTEX_BUFFER_VIEW* m_VertexBufferView = nullptr;

	struct ID3D12Resource* m_IndexBuffer = nullptr;
	struct D3D12_INDEX_BUFFER_VIEW* m_IndexBufferView = nullptr;
	// -メッシュ単位で持つもの

	// グラフィックパイプライン
	struct D3D12_GRAPHICS_PIPELINE_STATE_DESC* m_PipeLine = nullptr;
	struct ID3D12PipelineState* m_PipeLineState = nullptr;

	// ルートシグネチャ
	struct ID3D12RootSignature* m_RootSignature = nullptr;

	// ビューポート
	struct D3D12_VIEWPORT* m_Viewport = nullptr;

	// 切り抜き矩形
	D3D12_RECT m_ScissorRect = {};

	//// GIFactoryはDeviceと同じように他のオブジェクトを生み出すのに使うので
	//// 1つで良さそうだ。
	//struct IDXGIFactory6* m_GIFactory = nullptr;

	// スワップチェーンは画面の切り替え時に使うものなので、
	// こいつも1つで良さそう
	struct IDXGISwapChain4* m_SwapChain = nullptr;


	// BackBufferはRTVに使っているので、あれとセットだろう
	std::vector<struct ID3D12Resource*> m_BackBuffers;

	// フェンスとセット
	u64 m_FenceValue = 0;

};