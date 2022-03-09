#include "SwapChain.h"

#include <d3d12.h>
#include <dxgi1_6.h>

#include <eden/include/utility/ender_utility.h>

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>
#include <Atlantis/DirectX12/RenderTargetView/RenderTargetView.h>
#include <Atlantis/DirectX12/Command/CommandContext.h>
#include <Atlantis/DirectX12/MainDevice/MainDevice.h>

using namespace std;
EDENS_NAMESPACE_USING;
USING_ATLANTIS;

bool CSwapChain::Initialize(const FSwapChainInitializer& _Initializer)
{
	do
	{
		if (!CreateSwapChain(_Initializer)) { break; }
		if (!CreateRenderTargetView(_Initializer)) { break; }
		if (!CreateResourceBarrier()) { break; }

		return true;

	} while (0);

	return false;
}

void CSwapChain::Finalize()
{
	m_Barrier.reset();
	FinalizePtr(m_RenderTargetView);
	ReleaseD3DPtr(m_SwapChain);
}

void CSwapChain::Begin(CDX12MainDevice* _Device, CCommandContext* _Command)
{
	if (!_Command || !_Device) { return; }
	CRenderTargetView::BackBuffer* backBuffer = m_RenderTargetView->GetBackBuffer();
	if (!backBuffer) { return; }

	uint32 index = m_SwapChain->GetCurrentBackBufferIndex();

	m_Barrier->Transition.pResource = backBuffer->at(index);
	m_Barrier->Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	m_Barrier->Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	_Command->Barrier(1, m_Barrier.get());

	ID3D12DescriptorHeap* heap = m_RenderTargetView->GetRenderTargetView();
	if (!heap) { return; }

	m_Handle = heap->GetCPUDescriptorHandleForHeapStart();
	m_Handle.ptr += SCast<ULONG_PTR>(index * _Device->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	_Command->OMSetRenderTargets(1, &m_Handle, false, nullptr);
}

void CSwapChain::End(CCommandContext* _Command)
{
	if (!_Command) { return; }

	m_Barrier->Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	m_Barrier->Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	_Command->Barrier(1, m_Barrier.get());
}

void CSwapChain::SetRenderTargetView(CDX12MainDevice* _Device, CCommandContext* _Command)
{


}

void CSwapChain::Present()
{
	m_SwapChain->Present(1, 0);
}

bool CSwapChain::CreateSwapChain(const FSwapChainInitializer& _Initializer)
{
	if (!_Initializer.Factory) { return false; }
	if (!_Initializer.Queue) { return false; }

	DXGI_SWAP_CHAIN_DESC1 desc = {};

	desc.Width = _Initializer.ViewportWidth;
	desc.Height = _Initializer.ViewportHeight;

	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Stereo = false;

	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	desc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	desc.BufferCount = 2;

	desc.Scaling = DXGI_SCALING_STRETCH;

	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	IDXGISwapChain4* swapChain = nullptr;

	D3D_ERROR_CHECK(_Initializer.Factory->CreateSwapChainForHwnd(
		_Initializer.Queue,
		_Initializer.WindowHandle,
		&desc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)&swapChain));

	m_SwapChain = move(unique_ptr<IDXGISwapChain4>(swapChain));

	return true;
}

bool CSwapChain::CreateRenderTargetView(const FSwapChainInitializer& _Initializer)
{

	do
	{
		if (!_Initializer.Device) { break; }

		CRenderTargetView* renderTargetView = new CRenderTargetView();
		if (!renderTargetView) { break; };

		CRenderTargetView::FRenderTargetViewInitializer rtvInit;
		rtvInit.Device = _Initializer.Device;
		rtvInit.SwapChain = m_SwapChain.get();

		if (!renderTargetView->Initialize(rtvInit)) { break; }

		m_RenderTargetView.reset(renderTargetView);

		return true;
	} while (0);

	Finalize();

	return false;
}

bool CSwapChain::CreateResourceBarrier()
{
	D3D12_RESOURCE_BARRIER* desc = new D3D12_RESOURCE_BARRIER();
	if (!desc) { return false; }

	desc->Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	desc->Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	desc->Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	m_Barrier.reset(desc);

	return true;
}
