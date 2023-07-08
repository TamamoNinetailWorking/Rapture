#include "RHIProcessor.h"
#include "RHIProcessorInitialize.h"
#include "RHIRenderTargetDefine.h"

#include <eden/include/utility/ender_utility.h>

#include <Atlantis/DirectX12/DirectXDebug/DirectXDebug.h>
#include <Atlantis/DirectX12/MainDevice/MainDevice.h>
#include <Atlantis/DirectX12/Command/CommandContext.h>
#include <Atlantis/DirectX12/Command/CommandQueue.h>
#include <Atlantis/DirectX12/Barrier/Barrier.h>
#include <Atlantis/DirectX12/Fence/Fence.h>

#include <Atlantis/RHIProccessor/RHIRenderTargetView.h>
#include <Atlantis/SceneView/SceneView.h>
#include <Atlantis/DirectX12/SwapChain/SwapChain.h>
#include <Atlantis/DirectX12/RenderTargetView/RenderTargetView.h>

#include <Atlantis/Material/MaterialInterface.h>

USING_ATLANTIS;

ATLANTIS_NAMESPACE::CRHIProcessor::CRHIProcessor()
{
}

ATLANTIS_NAMESPACE::CRHIProcessor::~CRHIProcessor()
{
}

bool ATLANTIS_NAMESPACE::CRHIProcessor::Initialize(const FRHIProcessorInitialize* _Initializer)
{
	do
	{
		CHECK_RESULT_BREAK(_Initializer);

#ifdef ATLANTIS_DEBUG
		CHECK_RESULT_BREAK(CreateDebugLayer());
#endif

		CHECK_RESULT_BREAK(CreateDevice(_Initializer));
		CHECK_RESULT_BREAK(CreateCommandContext(_Initializer));
		CHECK_RESULT_BREAK(CreateCommandQueue(_Initializer));
		CHECK_RESULT_BREAK(CreateFence(_Initializer));
		CHECK_RESULT_BREAK(CreateBarrier(_Initializer));

		return true;
	} while (0);

	Finalize();
	return false;
}

void ATLANTIS_NAMESPACE::CRHIProcessor::Finalize()
{
	EDENS_NAMESPACE::FinalizeObject(m_Barrier);
	EDENS_NAMESPACE::FinalizeObject(m_Fence);
	EDENS_NAMESPACE::FinalizeObject(m_CommandQueue);
	EDENS_NAMESPACE::FinalizeObject(m_CommandContext);
	EDENS_NAMESPACE::FinalizeObject(m_Device);

#ifdef ATLANTIS_DEBUG
	EDENS_NAMESPACE::Delete(m_Debug);
#endif
}

bool ATLANTIS_NAMESPACE::CRHIProcessor::RenderBegin(const CRHIRenderTargetView* _RTV, const CSceneView* _View)
{
	CHECK_RESULT_FALSE(_RTV);
	CHECK_RESULT_FALSE(_View);

	uint32 bufferIndex = _RTV->GetSwapChain()->GetCurrentBufferIndex();
	auto rtv = _RTV->GetRenderTargetView(bufferIndex);
	CHECK_RESULT_FALSE(rtv);

	auto rtvResouce = rtv->GetResource();

	CBarrier::FTransitionState transitionState = {};
	transitionState.Resource = rtvResouce;
	transitionState.Before = Glue::EResourceState::RESOURCE_STATE_PRESENT;
	transitionState.After = Glue::EResourceState::RESOURCE_STATE_RENDER_TARGET;

	m_Barrier->SetTransitionState(transitionState);
	m_CommandContext->Barrier(m_Barrier);

	m_CommandContext->OMSetRenderTarget(rtv, _RTV->GetDepthStencilView());

	m_CommandContext->SetViewport(_View->GetViewport());
	m_CommandContext->SetScissorRect(_View->GetScissorRect());


	m_CommandContext->ClearRenderTargetView(rtv, RenderTargetClearColor);
	
	m_CommandContext->ClearDepthStencilView(_RTV->GetDepthStencilView(), DepthClearValue);

	return true;
}

void ATLANTIS_NAMESPACE::CRHIProcessor::RenderEnd(const CRHIRenderTargetView* _RTV)
{
	CHECK(_RTV);

	m_Barrier->SwapTransitionState();
	m_CommandContext->Barrier(m_Barrier);

	m_CommandContext->Close();


	m_CommandQueue->CommandListPush(m_CommandContext);
	m_CommandQueue->Execute();

	m_CommandQueue->Signal(m_Fence);

	m_Fence->WaitEvent();

	m_CommandContext->Reset();

	_RTV->GetSwapChain()->Present();
}

bool ATLANTIS_NAMESPACE::CRHIProcessor::SetPrimitiveTopology(Glue::EPrimitiveTopology _Topology)
{
	ID3D12GraphicsCommandList* CmdList = m_CommandContext->GetCommandList();
	CHECK_RESULT_FALSE(CmdList);

	CmdList->IASetPrimitiveTopology(Glue::GetPrimitiveTopology(_Topology));

	return true;
}

uint64 ATLANTIS_NAMESPACE::CRHIProcessor:: GetDescriptorHandleIncrementSize(Glue::EDescriptorHeapType _HeapType)
{
	return m_Device->GetDevice()->GetDescriptorHandleIncrementSize(Glue::GetDescriptorHeapType( _HeapType));
}

void ATLANTIS_NAMESPACE::CRHIProcessor::SetVertexBuffer(const CVertexBuffer* _VertexBuffer)
{
	m_CommandContext->SetVertexBuffer(_VertexBuffer);
}

void ATLANTIS_NAMESPACE::CRHIProcessor::SetIndexBuffer(const CIndexBuffer* _IndexBuffer)
{
	m_CommandContext->SetIndexBuffer(_IndexBuffer);
}

void ATLANTIS_NAMESPACE::CRHIProcessor::SetGraphicsPipeline(const IMaterialInterface* _Material)
{
	m_CommandContext->SetPipelineState(_Material->GetGraphicsPipeline());
	m_CommandContext->SetRootSignature(_Material->GetRootSignature());
}

bool ATLANTIS_NAMESPACE::CRHIProcessor::SetDescriptorHeaps(uint32 _Num, ID3D12DescriptorHeap* _DescHeap)
{
	ID3D12GraphicsCommandList* CmdList = m_CommandContext->GetCommandList();
	CHECK_RESULT_FALSE(CmdList);

	CmdList->SetDescriptorHeaps(_Num, &_DescHeap);

	return true;
}

bool ATLANTIS_NAMESPACE::CRHIProcessor::SetGraphicsRootDescriptorTable(uint32 _Offset,uint64 _Handle)
{
	ID3D12GraphicsCommandList* CmdList = m_CommandContext->GetCommandList();
	CHECK_RESULT_FALSE(CmdList);

	D3D12_GPU_DESCRIPTOR_HANDLE handle = {};
	handle.ptr = _Handle;

	CmdList->SetGraphicsRootDescriptorTable(_Offset, handle);

	return true;
}

bool ATLANTIS_NAMESPACE::CRHIProcessor::DrawIndexedInstanced(uint32 _CurrentIndex, uint32 _IndexOffset)
{
	ID3D12GraphicsCommandList* CmdList = m_CommandContext->GetCommandList();
	CHECK_RESULT_FALSE(CmdList);

	CmdList->DrawIndexedInstanced(_CurrentIndex, 1, _IndexOffset, 0, 0);
	
	return true;
}

#ifdef ATLANTIS_DEBUG
bool ATLANTIS_NAMESPACE::CRHIProcessor::CreateDebugLayer()
{
	m_Debug = new CDirectXDebug();
	CHECK_RESULT_FALSE(m_Debug);

	m_Debug->EnableDebugLayer();

	return true;
}
#endif

bool ATLANTIS_NAMESPACE::CRHIProcessor::CreateDevice(const FRHIProcessorInitialize* _Initializer)
{
	m_Device = new CDX12MainDevice();
	CHECK_RESULT_FALSE(m_Device);

	return m_Device->Initialize();
}

bool ATLANTIS_NAMESPACE::CRHIProcessor::CreateCommandContext(const FRHIProcessorInitialize* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	m_CommandContext = new CCommandContext();
	CHECK_RESULT_FALSE(m_CommandContext);

	CCommandContext::FCommandContextInitializer initializer = {};
	initializer.Device = m_Device->GetDevice();
	initializer.listType = _Initializer->CommandListType;
	initializer.NodeMask = 0;
	initializer.PipelineState = nullptr;

	return m_CommandContext->Initialize(initializer);
}

bool ATLANTIS_NAMESPACE::CRHIProcessor::CreateCommandQueue(const FRHIProcessorInitialize* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	m_CommandQueue = new CCommandQueue();
	CHECK_RESULT_FALSE(m_CommandQueue);

	CCommandQueue::FCommandQueueInitializer initializer = {};
	initializer.Device = m_Device->GetDevice();
	initializer.listType = _Initializer->CommandListType;
	initializer.QueueFlag = _Initializer->CommandQueueFlag;
	initializer.QueuePriority = _Initializer->CommandQueuePriority;
	initializer.NodeMask = 0;

	return m_CommandQueue->Initialize(initializer);
}

bool ATLANTIS_NAMESPACE::CRHIProcessor::CreateFence(const FRHIProcessorInitialize* _Initializer)
{
	m_Fence = new CFence();
	CHECK_RESULT_FALSE(m_Fence);

	CFence::FFenceInitializer initializer = {};
	initializer.Device = m_Device->GetDevice();

	return m_Fence->Initialize(initializer);
}

bool ATLANTIS_NAMESPACE::CRHIProcessor::CreateBarrier(const FRHIProcessorInitialize* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	m_Barrier = new CBarrier();
	CHECK_RESULT_FALSE(m_Barrier);

	CBarrier::FInitializer initializer = {};
	initializer.BarrierType = _Initializer->BarrierType;

	return m_Barrier->Initializer(initializer);
}
