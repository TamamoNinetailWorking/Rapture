#pragma once

#include <memory>
#include <vector>


struct ID3D12Device;
struct ID3D12DescriptorHeap;
struct ID3D12Resource;
struct IDXGISwapChain4;

ATLANTIS_NAMESPACE_BEGIN

class CRenderTargetView
{
public:

	struct FRenderTargetViewInitializer
	{
		ID3D12Device* Device = nullptr;
		IDXGISwapChain4* SwapChain = nullptr;
	};

	bool Initialize(const FRenderTargetViewInitializer& _Initializer);

	void Finalize();


	typedef std::vector<ID3D12Resource*> BackBuffer;

	// ゲッター
	ID3D12DescriptorHeap* GetRenderTargetView() const { return m_RenderTargetView.get(); };
	BackBuffer* GetBackBuffer() const { return m_BackBuffer.get(); };

private:


	bool CreateDescriptorHeap(const FRenderTargetViewInitializer& _Initializer);

	bool CreateRenderTargetView(const FRenderTargetViewInitializer& _Initializer);

	// レンダーターゲットビュー
	std::unique_ptr<ID3D12DescriptorHeap> m_RenderTargetView = nullptr;

	// バックバッファ
	std::unique_ptr<BackBuffer> m_BackBuffer = nullptr;
};


ATLANTIS_NAMESPACE_END