#pragma once

#include <memory>

#include <d3d12.h>
#include <dxgi1_6.h>

#include <Atlantis/DirectX12/RenderTargetView/RenderTargetView.h>

struct ID3D12Device;
struct ID3D12CommandQueue;
struct IDXGIFactory6;
struct IDXGISwapChain4;
struct D3D12_RESOURCE_BARRIER;
struct D3D12_RENDER_TARGET_VIEW_DESC;
struct ID3D12DescriptorHeap;

ATLANTIS_NAMESPACE_BEGIN

class CRenderTargetView;
class CCommandContext;
class CDX12MainDevice;

class CSwapChain
{
public:

	// 面倒なので一旦ここだけ。他は必要になったら変数化していこう
	struct FSwapChainInitializer
	{
		ID3D12Device* Device = nullptr;
		IDXGIFactory6* Factory = nullptr;
		ID3D12CommandQueue* Queue = nullptr;
		HWND WindowHandle = {};
		D3D12_RENDER_TARGET_VIEW_DESC* RtvDesc = nullptr;
		uint32 ViewportWidth = 0;
		uint32 ViewportHeight = 0;
		uint32 BufferCount = 2;
	};

	// 初期化
	bool Initialize(const FSwapChainInitializer& _Initializer);

	// 終了処理
	void Finalize();

	// （一時的）深度バッファの設定
	void SetDepthBuffer(ID3D12DescriptorHeap* _DepthBuffer);

	// 描画準備
	void Begin(CDX12MainDevice* _Device, CCommandContext* _Command);

	void End(CCommandContext* _Command);

	// レンダーターゲットビューのセット
	void SetRenderTargetView(CDX12MainDevice* _Device,CCommandContext* _Command);


	// フリップ
	void Present();


	// ゲッター
	IDXGISwapChain4* GetSwapChain() const { return m_SwapChain.get(); };

	const D3D12_CPU_DESCRIPTOR_HANDLE& GetRenderTargetViewHandle() const { return m_Handle; };


	CSwapChain() {};
	~CSwapChain() {};

private:

	bool CreateSwapChain(const FSwapChainInitializer& _Initializer);

	bool CreateRenderTargetView(const FSwapChainInitializer& _Initializer);

	bool CreateResourceBarrier();

	// スワップチェーン
	std::unique_ptr<IDXGISwapChain4> m_SwapChain = nullptr;

	// レンダーターゲットビュー
	std::unique_ptr<CRenderTargetView> m_RenderTargetView = nullptr;

	// レンダーターゲットのリソースバリア
	std::unique_ptr<D3D12_RESOURCE_BARRIER> m_Barrier = nullptr;

	// ヒープハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE m_Handle = {};

	// 深度バッファ
	std::unique_ptr<ID3D12DescriptorHeap> m_DepthBuffer = nullptr;

};


ATLANTIS_NAMESPACE_END