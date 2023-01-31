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
	if (m_IsResouceMine)
	{
		SafeReleaseD3DPtr(m_Resource);
	}
	SafeReleaseD3DPtr(m_MaterialDescriptorHeap);
}

bool CRenderTargetView::CreateDescriptorHeap(const FRenderTargetViewInitializer& _Initializer)
{
	if (!_Initializer.Device) { return false; }

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットを作成しているようだ
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // 特にフラグの指定は無し

	ID3D12DescriptorHeap* renderTargetView = nullptr;
	D3D_ERROR_CHECK(_Initializer.Device->CreateDescriptorHeap(
		&heapDesc, 
		IID_PPV_ARGS(&renderTargetView)));

	m_MaterialDescriptorHeap = renderTargetView;

	return true;
}

bool CRenderTargetView::CreateRenderTargetView(const FRenderTargetViewInitializer& _Initializer)
{
	if (!_Initializer.Device) { return false; }

	if (_Initializer.ResPtr != nullptr)
	{
		m_IsResouceMine = false;
		m_Resource = _Initializer.ResPtr;
	}

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = Glue::GetDXGIFormat(_Initializer.Format);
	rtvDesc.ViewDimension = Glue::GetD3DRTVDimension(_Initializer.RTVDimension);

	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_MaterialDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	_Initializer.Device->CreateRenderTargetView(m_Resource, &rtvDesc, handle);

	return true;
}
