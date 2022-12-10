#include "RenderTargetView.h"

#include <d3d12.h>
#include <dxgi1_6.h>

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>

using namespace std;
USING_ATLANTIS;

bool CRenderTargetView::Initialize(const FRenderTargetViewInitializer& _Initializer)
{
	do
	{
		if (!CreateDescriptorHeap(_Initializer)) { break; }
		if (!CreateRenderTargetView(_Initializer)) { break; }

		return true;
	} while (0);

	Finalize();
	return false;
}

void CRenderTargetView::Finalize()
{
	if (!m_BackBuffer)
	{
		// 配列なので特殊
		for (auto itr = m_BackBuffer->begin(); itr != m_BackBuffer->end();++itr)
		{
			(*itr)->Release();
		}
		m_BackBuffer->resize(0);
		m_BackBuffer.reset();
	}

	ReleaseD3DPtr(m_RenderTargetView);
}

bool CRenderTargetView::CreateDescriptorHeap(const FRenderTargetViewInitializer& _Initializer)
{
	if (!_Initializer.Device) { return false; }

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットを作成しているようだ
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = _Initializer.BackBufferCount; // 表裏の2つ
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // 特にフラグの指定は無し

	ID3D12DescriptorHeap* renderTargetView = nullptr;
	D3D_ERROR_CHECK(_Initializer.Device->CreateDescriptorHeap(
		&heapDesc, 
		IID_PPV_ARGS(&renderTargetView)));

	m_RenderTargetView = move(unique_ptr<ID3D12DescriptorHeap>(renderTargetView));

	return true;
}

bool CRenderTargetView::CreateRenderTargetView(const FRenderTargetViewInitializer& _Initializer)
{
	if (!_Initializer.Device) { return false; }
	if (!_Initializer.SwapChain) { return false; }

	//DXGI_SWAP_CHAIN_DESC desc = {};
	//D3D_ERROR_CHECK(_Initializer.SwapChain->GetDesc(&desc));

	BackBuffer* backBuffer = new BackBuffer();
	if (!backBuffer) { return false; }
	//backBuffer->resize(desc.BufferCount);
	backBuffer->resize(_Initializer.BackBufferCount);

	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_RenderTargetView->GetCPUDescriptorHandleForHeapStart();

	for (uint32 i = 0; i < backBuffer->size(); ++i)
	{
		D3D_ERROR_CHECK(_Initializer.SwapChain->GetBuffer(
			i,
			IID_PPV_ARGS(&backBuffer->at(i))));

		_Initializer.Device->CreateRenderTargetView(backBuffer->at(i), nullptr, handle);
		handle.ptr += _Initializer.Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		
	}

	m_BackBuffer.reset(backBuffer);

	
	return true;
}
