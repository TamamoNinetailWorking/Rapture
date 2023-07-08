#pragma once

#include <Atlantis/DirectX12/DirectXPaste.h>

ATLANTIS_NAMESPACE_BEGIN

class CDX12MainDevice;
class CCommandContext;
class CCommandQueue;
class CSwapChain;
class CFence;
class CBarrier;

#ifdef ATLANTIS_DEBUG
class CDirectXDebug;
#endif

class CVertexBuffer;
class CIndexBuffer;
class IMaterialInterface;

struct FRHIProcessorInitialize;

class CRHIRenderTargetView;
class CSceneView;

class CRHIProcessor
{
public:

	bool Initialize(const FRHIProcessorInitialize* _Initializer);
	void Finalize();

	bool RenderBegin(const CRHIRenderTargetView* _RTV, const CSceneView* _View);
	void RenderEnd(const CRHIRenderTargetView* _RTV);

	const CDX12MainDevice* GetDevice() const { return m_Device; }
	CDX12MainDevice* GetDeviceEdit() const { return m_Device; }

	const CCommandQueue* GetQueue() const { return m_CommandQueue; };
	CCommandQueue* GetQueueEdit() const { return m_CommandQueue; };

	const CCommandContext* GetContext() const { return m_CommandContext; };
	CCommandContext* GetContextEdit() const { return m_CommandContext; };

	CRHIProcessor();
	~CRHIProcessor();

public:

	uint64 GetDescriptorHandleIncrementSize(Glue::EDescriptorHeapType _HeapType);

	bool SetPrimitiveTopology(Glue::EPrimitiveTopology _Topology);
	void SetVertexBuffer(const CVertexBuffer* _VertexBuffer);
	void SetIndexBuffer(const CIndexBuffer* _IndexBuffer);
	void SetGraphicsPipeline(const IMaterialInterface* _Material);
	bool SetDescriptorHeaps(uint32 _Num, ID3D12DescriptorHeap* _DescHeap);
	bool SetGraphicsRootDescriptorTable(uint32 _Offset,uint64 _Handle);

	bool DrawIndexedInstanced(uint32 _CurrentIndex, uint32 _IndexOffset);

private:

	ObjectPtr(CDX12MainDevice) m_Device = nullptr;
	ObjectPtr(CCommandContext) m_CommandContext = nullptr;
	ObjectPtr(CCommandQueue) m_CommandQueue = nullptr;
	ObjectPtr(CFence) m_Fence = nullptr;
	ObjectPtr(CBarrier) m_Barrier = nullptr;

#ifdef ATLANTIS_DEBUG
	ObjectPtr(CDirectXDebug) m_Debug = nullptr;
#endif

private:

#ifdef ATLANTIS_DEBUG
	bool CreateDebugLayer();
#endif

	bool CreateDevice(const FRHIProcessorInitialize* _Initializer);
	bool CreateCommandContext(const FRHIProcessorInitialize* _Initializer);
	bool CreateCommandQueue(const FRHIProcessorInitialize* _Initializer);
	bool CreateFence(const FRHIProcessorInitialize* _Initializer);
	bool CreateBarrier(const FRHIProcessorInitialize* _Initializer);

};

ATLANTIS_NAMESPACE_END