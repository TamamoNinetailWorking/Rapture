#include "CommandContext.h"

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>

#include <Atlantis/DirectX12/Viewport/Viewport.h>
#include <Atlantis/DirectX12/ScissorRect/ScissorRect.h>
#include <Atlantis/DirectX12/GraphicsPipeline/GraphicsPipeline.h>
#include <Atlantis/DirectX12/RootSignature/RootSignature.h>

using namespace std;

USING_ATLANTIS;
USING_GLUE;

bool CCommandContext::Initialize(const FCommandContextInitializer& _Initializer)
{
	do
	{
		if (!_Initializer.Device) { break; };

		if (!CreateCommandAllocator(_Initializer)) { break; }

		if (!CreateCommandList(_Initializer)) { break; }

		return true;

	} while (0);

	Finalize();

	return false;
}


void CCommandContext::Finalize()
{
	ReleaseD3DPtr(m_CmdList);
	ReleaseD3DPtr(m_CmdAllocator);
}

void CCommandContext::Barrier(uint32 _Num, D3D12_RESOURCE_BARRIER* _Resource)
{
	if (!_Resource) { return; }
	m_CmdList->ResourceBarrier(_Num, _Resource);
}

void CCommandContext::OMSetRenderTargets(uint32 NumRenderTargetDescriptors, const D3D12_CPU_DESCRIPTOR_HANDLE* pRenderTargetDescriptors, bool RTsSingleHandleToDescriptorRange, const D3D12_CPU_DESCRIPTOR_HANDLE* pDepthStencilDescriptor)
{
	m_CmdList->OMSetRenderTargets(NumRenderTargetDescriptors, pRenderTargetDescriptors, RTsSingleHandleToDescriptorRange, pDepthStencilDescriptor);
}

void CCommandContext::SetViewport(CViewport* _viewport)
{
	D3D_CHECK(_viewport);
	m_CmdList->RSSetViewports(1, _viewport->GetViewport());
}

void CCommandContext::SetScissorRect(CScissorRect* _rect)
{
	D3D_CHECK(_rect);
	m_CmdList->RSSetScissorRects(1, _rect->GetScissorRect());
}

void CCommandContext::SetPipelineState(CGraphicsPipeline* _pipeline)
{
	D3D_CHECK(_pipeline);
	m_CmdList->SetPipelineState(_pipeline->GetPipelineState());
}

void CCommandContext::SetRootSignature(CRootSignature* _RootSignature)
{
	D3D_CHECK(_RootSignature);
	m_CmdList->SetGraphicsRootSignature(_RootSignature->GetRootSignature());
}

void CCommandContext::Close()
{
	m_CmdList->Close();
}


void CCommandContext::Reset(ID3D12PipelineState* _PipelineState)
{
	m_CmdAllocator->Reset();
	m_CmdList->Reset(m_CmdAllocator.get(), _PipelineState);
}


bool CCommandContext::CreateCommandAllocator(const FCommandContextInitializer& _Initializer)
{

	ID3D12CommandAllocator* allocator = nullptr;
	D3D_ERROR_CHECK(_Initializer.Device->CreateCommandAllocator(
		GetD3DCommandListType(_Initializer.listType),
		IID_PPV_ARGS(&allocator)));

	m_CmdAllocator = move(unique_ptr<ID3D12CommandAllocator>(allocator));
	return true;
}


bool CCommandContext::CreateCommandList(const FCommandContextInitializer& _Initializer)
{
	ID3D12GraphicsCommandList* list = nullptr;
	D3D_ERROR_CHECK(_Initializer.Device->CreateCommandList(
		_Initializer.NodeMask,
		GetD3DCommandListType(_Initializer.listType),
		m_CmdAllocator.get(),
		_Initializer.PipelineState,
		IID_PPV_ARGS(&list)));

	m_CmdList = move(unique_ptr<ID3D12GraphicsCommandList>(list));
	return true;
}
