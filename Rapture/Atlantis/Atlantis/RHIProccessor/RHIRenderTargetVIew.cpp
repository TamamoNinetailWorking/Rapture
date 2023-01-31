#include "RHIRenderTargetView.h"
#include "RHIRenderTargetDefine.h"
#include "RHIRenderTargetViewInitializer.h"

#include <eden/include/utility/ender_utility.h>

#include <Atlantis/DirectX12/MainDevice/MainDevice.h>
#include <Atlantis/DirectX12/Command/CommandQueue.h>

#include <Atlantis/DirectX12/SwapChain/SwapChain.h>
#include <Atlantis/DirectX12/RenderTargetView/RenderTargetView.h>
#include <Atlantis/DirectX12/DepthStencilView/DepthStencilView.h>

USING_ATLANTIS;

ATLANTIS_NAMESPACE::CRHIRenderTargetView::CRHIRenderTargetView()
{
}

ATLANTIS_NAMESPACE::CRHIRenderTargetView::~CRHIRenderTargetView()
{
}

bool ATLANTIS_NAMESPACE::CRHIRenderTargetView::Initialize(const FRHIRenderTargetInitializer* _Initializer)
{
	do
	{
		CHECK_RESULT_BREAK(_Initializer);

		CHECK_RESULT_BREAK(CreateSwapChain(_Initializer));
		CHECK_RESULT_BREAK(CreateRenderTargetView(_Initializer));
		CHECK_RESULT_BREAK(CreateDepthStencilView(_Initializer));

		return true;

	} while (0);

	Finalize();
	return false;
}

void ATLANTIS_NAMESPACE::CRHIRenderTargetView::Finalize()
{
	EDENS_NAMESPACE::FinalizeObject(m_DepthStencilView);

	for (auto& elem : m_RTVBuffer)
	{
		EDENS_NAMESPACE::FinalizeObject(elem);
	}

	EDENS_NAMESPACE::FinalizeObject(m_SwapChain);
}

bool ATLANTIS_NAMESPACE::CRHIRenderTargetView::CreateSwapChain(const FRHIRenderTargetInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(_Initializer->Device);
	CHECK_RESULT_FALSE(_Initializer->CommandQueue);

	m_SwapChain = new CSwapChain();
	CHECK_RESULT_FALSE(m_SwapChain);

	CSwapChain::FSwapChainInitializer initializer = {};
	initializer.Device = _Initializer->Device->GetDevice();
	initializer.Factory = _Initializer->Device->GetGIFactory();
	initializer.Queue = _Initializer->CommandQueue->GetCommandQueue();
	initializer.ViewportWidth = _Initializer->ViewWidth;
	initializer.ViewportHeight = _Initializer->ViewHeight;
	initializer.WindowHandle = _Initializer->WindowHandle;

	initializer.BufferCount = RHI_RenderTargetViewNum;

	return m_SwapChain->Initialize(initializer);
}

bool ATLANTIS_NAMESPACE::CRHIRenderTargetView::CreateRenderTargetView(const FRHIRenderTargetInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(_Initializer->Device);

	CRenderTargetView::FRenderTargetViewInitializer initializer = {};
	initializer.Device = _Initializer->Device->GetDevice();
	initializer.Format = _Initializer->Format;
	initializer.RTVDimension = _Initializer->RTVDimension;

	for (uint32 i = 0; i < RHI_RenderTargetViewNum; ++i)
	{
		auto& elem = m_RTVBuffer.at(i);

		initializer.ResPtr = m_SwapChain->GetBackBuffer()->at(i);

		elem = new CRenderTargetView();
		CHECK_RESULT_FALSE(elem);

		CHECK_RESULT_FALSE(elem->Initialize(initializer));
	}

	return true;
}

bool ATLANTIS_NAMESPACE::CRHIRenderTargetView::CreateDepthStencilView(const FRHIRenderTargetInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(_Initializer->Device);

	m_DepthStencilView = new CDepthStencilView();
	CHECK_RESULT_FALSE(m_DepthStencilView);

	CDepthStencilView::FInitializer initializer = {};
	initializer.Device = _Initializer->Device->GetDevice();
	initializer.Width = _Initializer->ViewWidth;
	initializer.Height = _Initializer->ViewHeight;
	
	initializer.ClearDepth = DepthClearValue;

	return m_DepthStencilView->Initialize(initializer);
}
