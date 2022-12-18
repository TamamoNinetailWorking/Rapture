#pragma once

#include <memory>
//#include <d3d12.h>

#include <Atlantis/DirectX12/DirectXPaste.h>

ATLANTIS_NAMESPACE_BEGIN

class CCommandContext
{
public:

	struct FCommandContextInitializer
	{
		ID3D12Device* Device = nullptr;
		ID3D12PipelineState* PipelineState = nullptr;
		uint32 NodeMask = 0;
		Glue::ECommandListType listType;
	};

	bool Initialize(const FCommandContextInitializer& _Initializer);
	void Finalize();

	// リソースバリア
	void Barrier(uint32 _Num, D3D12_RESOURCE_BARRIER* _Resource);

	// OMSetRenderTargets
	void OMSetRenderTargets(
		uint32 NumRenderTargetDescriptors,
		const D3D12_CPU_DESCRIPTOR_HANDLE* pRenderTargetDescriptors,
		bool RTsSingleHandleToDescriptorRange,
		const D3D12_CPU_DESCRIPTOR_HANDLE* pDepthStencilDescriptor);

	// RSSetViewport
	void SetViewport(class CViewport* _viewport);

	// RSSetScissorRect
	void SetScissorRect(class CScissorRect* _rect);

	// SetPipelineState
	void SetPipelineState(class CGraphicsPipeline* _pipeline);

	// SetRootSignature
	void SetRootSignature(class CRootSignature* _RootSignature);

	// クローズ
	void Close();

	// リセット
	void Reset(ID3D12PipelineState* _PipelineState = nullptr);

	// ゲッター
	ID3D12CommandAllocator* GetCommandAllocator() const { return m_CmdAllocator; };
	ID3D12GraphicsCommandList* GetCommandList() const { return m_CmdList; };



	CCommandContext() {};
	~CCommandContext() {};

private:

	bool CreateCommandAllocator(const FCommandContextInitializer& _Initializer);

	bool CreateCommandList(const FCommandContextInitializer& _Initializer);


	// コマンドアロケーター（コマンドの実体を保有する）
	//std::unique_ptr<ID3D12CommandAllocator> m_CmdAllocator = nullptr;
	ObjectPtr(ID3D12CommandAllocator) m_CmdAllocator = nullptr;

	// コマンドリスト（コマンドの仮想リスト）
	//std::unique_ptr<ID3D12GraphicsCommandList> m_CmdList = nullptr;
	ObjectPtr(ID3D12GraphicsCommandList) m_CmdList = nullptr;

};


ATLANTIS_NAMESPACE_END