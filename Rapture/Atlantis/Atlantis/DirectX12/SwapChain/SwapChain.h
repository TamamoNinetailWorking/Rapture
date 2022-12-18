#pragma once

#include <memory>

#include <d3d12.h>
#include <dxgi1_6.h>

#include <Atlantis/DirectX12/RenderTargetView/RenderTargetView.h>

struct ID3D12Device;
struct ID3D12CommandQueue;
struct IDXGIFactory6;
struct IDXGISwapChain4;

ATLANTIS_NAMESPACE_BEGIN

class CCommandContext;
class CDX12MainDevice;

class CSwapChain
{
public:

	typedef std::vector<ID3D12Resource*> BackBuffer;

	// 面倒なので一旦ここだけ。他は必要になったら変数化していこう
	struct FSwapChainInitializer
	{
		ID3D12Device* Device = nullptr;
		IDXGIFactory6* Factory = nullptr;
		ID3D12CommandQueue* Queue = nullptr;
		HWND WindowHandle = {};
		uint32 ViewportWidth = 0;
		uint32 ViewportHeight = 0;
		uint32 BufferCount = 2;
	};

	// 初期化
	bool Initialize(const FSwapChainInitializer& _Initializer);

	// 終了処理
	void Finalize();

	// フリップ
	void Present();


	// ゲッター
	IDXGISwapChain4* GetSwapChain() const { return m_SwapChain; };

	BackBuffer* GetBackBuffer() const { return m_BackBuffer; };

	uint32 GetCurrentBufferIndex() const { return m_SwapChain->GetCurrentBackBufferIndex(); };


	CSwapChain() {};
	~CSwapChain() {};

private:

	bool CreateSwapChain(const FSwapChainInitializer& _Initializer);

	bool CreateBackBuffer(const FSwapChainInitializer& _Initializer);

	// スワップチェーン
	//std::unique_ptr<IDXGISwapChain4> m_SwapChain = nullptr;
	ObjectPtr(IDXGISwapChain4) m_SwapChain = nullptr;

	// バックバッファ
	ObjectPtr(BackBuffer) m_BackBuffer = nullptr;

};


ATLANTIS_NAMESPACE_END