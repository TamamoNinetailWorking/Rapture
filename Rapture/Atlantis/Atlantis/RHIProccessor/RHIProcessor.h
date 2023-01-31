#pragma once

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