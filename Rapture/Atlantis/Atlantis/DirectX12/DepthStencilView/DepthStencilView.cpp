#include "DepthStencilView.h"

#include <d3d12.h>
#include <Atlantis/DirectX12/DirectXHelper/d3dx12.h>

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>


USING_ATLANTIS;


bool ATLANTIS_NAMESPACE::CDepthStencilView::Initialize(const FInitializer& _Initializer)
{
	do
	{
		D3D_INIT_PROCESS_CHECK(CreateDescriptorHeap(_Initializer));
		D3D_INIT_PROCESS_CHECK(CreateResource(_Initializer));
		D3D_INIT_PROCESS_CHECK(CreateDepthStencilView(_Initializer));

		return true;
	} while (0);

	return false;
}

void ATLANTIS_NAMESPACE::CDepthStencilView::Finalize()
{
	SafeReleaseD3DPtr(m_Resource);
	SafeReleaseD3DPtr(m_DescriptorHeap);
}

bool ATLANTIS_NAMESPACE::CDepthStencilView::CreateDescriptorHeap(const FInitializer& _Initializer)
{
	D3D_CHECK_NULL(_Initializer.Device);

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	D3D_ERROR_CHECK(_Initializer.Device->CreateDescriptorHeap(
		&desc,
		IID_PPV_ARGS(&m_DescriptorHeap)
	));

	return true;
}

bool ATLANTIS_NAMESPACE::CDepthStencilView::CreateResource(const FInitializer& _Initializer)
{
	D3D_CHECK_NULL(_Initializer.Device);

	auto format = DXGI_FORMAT_D32_FLOAT;

	D3D12_HEAP_PROPERTIES prop = {};
	prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Width = _Initializer.Width;
	desc.Height = _Initializer.Height;
	desc.DepthOrArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.DepthStencil.Depth = _Initializer.ClearDepth;
	clearValue.Format = format;

	D3D_ERROR_CHECK(_Initializer.Device->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		IID_PPV_ARGS(&m_Resource)
	));

	return true;
}

bool ATLANTIS_NAMESPACE::CDepthStencilView::CreateDepthStencilView(const FInitializer& _Initializer)
{
	D3D_CHECK_NULL(_Initializer.Device);

	D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
	desc.Format = DXGI_FORMAT_D32_FLOAT;
	desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	desc.Flags = D3D12_DSV_FLAG_NONE;

	_Initializer.Device->CreateDepthStencilView(
		m_Resource,
		&desc,
		m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart()
	);

	return true;
}
