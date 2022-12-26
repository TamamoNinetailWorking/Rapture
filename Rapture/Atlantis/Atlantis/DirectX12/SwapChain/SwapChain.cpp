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
		D3D_INIT_PROCESS_CHECK(CreateBackBuffer(_Initializer));

		return true;

	} while (0);

	return false;
}

void CSwapChain::Finalize()
{
	SafeReleaseD3DPtr(m_SwapChain);
}

void CSwapChain::Present()
{
	m_SwapChain->Present(1, 0);
}

uint32 ATLANTIS_NAMESPACE::CSwapChain::GetCurrentBufferIndex() const
{
	return m_SwapChain->GetCurrentBackBufferIndex();
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

	//desc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = _Initializer.BufferCount;

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

	//m_SwapChain = move(unique_ptr<IDXGISwapChain4>(swapChain));
	m_SwapChain = swapChain;

	return true;
}

bool ATLANTIS_NAMESPACE::CSwapChain::CreateBackBuffer(const FSwapChainInitializer& _Initializer)
{
	m_BackBuffer = new BackBuffer();
	D3D_CHECK_NULL(m_BackBuffer);
	m_BackBuffer->resize(_Initializer.BufferCount);

	for (uint32 i = 0; i < m_BackBuffer->size(); ++i)
	{
		D3D_ERROR_CHECK(m_SwapChain->GetBuffer(
			i, 
			IID_PPV_ARGS(&m_BackBuffer->at(i))
		));
	}
	return true;
}

