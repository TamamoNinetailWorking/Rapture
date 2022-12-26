#include "CommandContext.h"

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>

#include <Atlantis/DirectX12/Viewport/Viewport.h>
#include <Atlantis/DirectX12/ScissorRect/ScissorRect.h>
#include <Atlantis/DirectX12/GraphicsPipeline/GraphicsPipeline.h>
#include <Atlantis/DirectX12/RootSignature/RootSignature.h>
#include <Atlantis/DirectX12/Barrier/Barrier.h>
#include <Atlantis/DirectX12/RenderTargetView/RenderTargetView.h>
#include <Atlantis/DirectX12/DepthStencilView/DepthStencilView.h>

#include <Atlantis/DirectX12/VertexBuffer/VertexBuffer.h>
#include <Atlantis/DirectX12/IndexBuffer/IndexBuffer.h>

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
	//ReleaseD3DUniquePtr(m_CmdList);
	SafeReleaseD3DPtr(m_CmdList);
	//ReleaseD3DUniquePtr(m_CmdAllocator);
	SafeReleaseD3DPtr(m_CmdAllocator);
}

void CCommandContext::Barrier(const CBarrier* _Barrier)
{
	D3D_CHECK(_Barrier);
	//m_CmdList->ResourceBarrier(_Num, _Resource);
	m_CmdList->ResourceBarrier(1, _Barrier->GetBarrier());
}

void CCommandContext::OMSetRenderTarget(CRenderTargetView* _RenderTarget, CDepthStencilView* _DepthView)
{
	D3D_CHECK(_RenderTarget);

	auto rtvHandle = _RenderTarget->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	
	D3D12_CPU_DESCRIPTOR_HANDLE* depthHandle = nullptr;
	if (_DepthView != nullptr)
	{
		auto temp = _DepthView->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
		depthHandle = &temp;
	}

	m_CmdList->OMSetRenderTargets(1, &rtvHandle, false, depthHandle);
}

void CCommandContext::SetViewport(const CViewport* _viewport)
{
	D3D_CHECK(_viewport);
	m_CmdList->RSSetViewports(1, _viewport->GetViewport());
}

void CCommandContext::SetScissorRect(const CScissorRect* _rect)
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

void ATLANTIS_NAMESPACE::CCommandContext::ClearRenderTargetView(CRenderTargetView* _RenderTarget, const float* _ClearColor)
{
	D3D_CHECK(_RenderTarget);
	auto handle = _RenderTarget->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	m_CmdList->ClearRenderTargetView(handle, _ClearColor, 0, nullptr);
}

void ATLANTIS_NAMESPACE::CCommandContext::ClearDepthStencilView(CDepthStencilView* _DepthStencilView, float _ClearDepth)
{
	D3D_CHECK(_DepthStencilView);
	auto handle = _DepthStencilView->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	m_CmdList->ClearDepthStencilView(handle, D3D12_CLEAR_FLAG_DEPTH, _ClearDepth, 0, 0, nullptr);
}

void ATLANTIS_NAMESPACE::CCommandContext::SetVertexBuffer(const CVertexBuffer* _VertexBuffer)
{
	D3D_CHECK(_VertexBuffer);
	m_CmdList->IASetVertexBuffers(0, 1, _VertexBuffer->GetVertexBufferView());
}

void ATLANTIS_NAMESPACE::CCommandContext::SetIndexBuffer(const CIndexBuffer* _IndexBuffer)
{
	D3D_CHECK(_IndexBuffer);
	m_CmdList->IASetIndexBuffer(_IndexBuffer->GetIndexBufferView());
}

void CCommandContext::Close()
{
	m_CmdList->Close();
}


void CCommandContext::Reset(CGraphicsPipeline* _Pipeline)
{
	m_CmdAllocator->Reset();
	//m_CmdList->Reset(m_CmdAllocator.get(), _PipelineState);
	m_CmdList->Reset(m_CmdAllocator, _Pipeline->GetPipelineState());
}


bool CCommandContext::CreateCommandAllocator(const FCommandContextInitializer& _Initializer)
{
	ID3D12CommandAllocator* allocator = nullptr;
	D3D_ERROR_CHECK(_Initializer.Device->CreateCommandAllocator(
		GetD3DCommandListType(_Initializer.listType),
		IID_PPV_ARGS(&allocator)));

	//m_CmdAllocator = move(unique_ptr<ID3D12CommandAllocator>(allocator));
	m_CmdAllocator = allocator;
	return true;
}


bool CCommandContext::CreateCommandList(const FCommandContextInitializer& _Initializer)
{
	ID3D12GraphicsCommandList* list = nullptr;
	D3D_ERROR_CHECK(_Initializer.Device->CreateCommandList(
		_Initializer.NodeMask,
		GetD3DCommandListType(_Initializer.listType),
		m_CmdAllocator,
		_Initializer.PipelineState,
		IID_PPV_ARGS(&list)));

	//m_CmdList = move(unique_ptr<ID3D12GraphicsCommandList>(list));
	m_CmdList = list;
	return true;
}
