#pragma once

//#include <memory>
//#include <vector>


struct ID3D12Device;
struct ID3D12DescriptorHeap;
struct ID3D12Resource;
struct IDXGISwapChain4;
struct D3D12_RENDER_TARGET_VIEW_DESC;

ATLANTIS_NAMESPACE_BEGIN

class CRenderTargetView
{
public:

	struct FRenderTargetViewInitializer
	{
		ID3D12Device* Device = nullptr;
		ID3D12Resource* ResPtr = nullptr;
		D3D12_RENDER_TARGET_VIEW_DESC* RtvDesc = nullptr;
	};

	bool Initialize(const FRenderTargetViewInitializer& _Initializer);

	void Finalize();

	// ゲッター
	ID3D12DescriptorHeap* GetDescriptorHeap() const { return m_DescriptorHeap; };
	ID3D12Resource* GetResource() const { return m_Resource; };

	CRenderTargetView() {};
	~CRenderTargetView() {};

private:


	bool CreateDescriptorHeap(const FRenderTargetViewInitializer& _Initializer);

	bool CreateRenderTargetView(const FRenderTargetViewInitializer& _Initializer);

	// ディスクリプタ―ヒープ
	ObjectPtr(ID3D12DescriptorHeap) m_DescriptorHeap = nullptr;

	// RTVリソース
	ObjectPtr(ID3D12Resource) m_Resource = nullptr;

	// RTVリソースが自身のものかどうか
	// >>解放責任が自分にあるかどうか
	bool m_IsResouceMine = true;
};


ATLANTIS_NAMESPACE_END