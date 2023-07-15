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

	// RTV
	{
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットを作成しているようだ
		heapDesc.NodeMask = 0;
		heapDesc.NumDescriptors = 1;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // 特にフラグの指定は無し

		ID3D12DescriptorHeap* renderTargetView = nullptr;
		D3D_ERROR_CHECK(_Initializer.Device->CreateDescriptorHeap(
			&heapDesc,
			IID_PPV_ARGS(&renderTargetView)));

		m_MaterialDescriptorHeap = renderTargetView;
	}

	// SRV
	{
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.NumDescriptors = 1;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; //SRVなのでシェーダーから見えるように
		
		ID3D12DescriptorHeap* shaderResourceView = nullptr;
		D3D_ERROR_CHECK(_Initializer.Device->CreateDescriptorHeap(
			&heapDesc,
			IID_PPV_ARGS(&shaderResourceView)));

		m_ShaderResourceView = shaderResourceView;
	}

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
	
	// RTV
	{
		rtvDesc.Format = Glue::GetDXGIFormat(_Initializer.Format);
		rtvDesc.ViewDimension = Glue::GetD3DRTVDimension(_Initializer.RTVDimension);

		D3D12_CPU_DESCRIPTOR_HANDLE handle = m_MaterialDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

		_Initializer.Device->CreateRenderTargetView(m_Resource, &rtvDesc, handle);
	}

	// SRV
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Format = rtvDesc.Format;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		D3D12_CPU_DESCRIPTOR_HANDLE handle = m_ShaderResourceView->GetCPUDescriptorHandleForHeapStart();

		_Initializer.Device->CreateShaderResourceView(m_Resource, &srvDesc, handle);
	}

	return true;
}
