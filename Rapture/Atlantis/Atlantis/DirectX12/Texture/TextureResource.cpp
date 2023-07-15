#include "TextureResource.h"
#include "TextureResourceInitializer.h"

#include <Atlantis/DirectX12/DirectXHelper/d3dx12.h>
#include <Atlantis/DirectX12/DirectX12BaseDefine.h>

USING_ATLANTIS;
EDENS_NAMESPACE_USING;

bool CTextureResource::Initialize(const FResourceInitializerBase* _initializer)
{
	do
	{
		const FTextureInitializer* initializer = PCast<const FTextureInitializer*>(_initializer);

		CHECK_RESULT_BREAK(CreateTextureResource(initializer));
		CHECK_RESULT_BREAK(CopyToVideoMemory(initializer));

		return true;
	} while (0);

	Finalize();
	return false;
}

void CTextureResource::Finalize()
{
	SafeReleaseD3DPtr(m_Resource);
}

bool CTextureResource::CreateTextureResource(const FTextureInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(_Initializer->Device);

	D3D12_HEAP_PROPERTIES prop = {};
	switch (_Initializer->HeapType)
	{
	case Glue::EHeapType::CUSTOM:
		
		prop.Type = D3D12_HEAP_TYPE_CUSTOM;
		prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
		prop.CreationNodeMask = 0;
		prop.VisibleNodeMask = 0;
		
		break;

	default:

		prop = CD3DX12_HEAP_PROPERTIES(Glue::GetHeapType(_Initializer->HeapType));

		break;
	}


	D3D12_RESOURCE_DESC desc = {};
	if (!_Initializer->pResDesc)
	{
		desc.Format = Glue::GetDXGIFormat(_Initializer->ResDesc.Format);
		desc.Width = _Initializer->ResDesc.Width;
		desc.Height = _Initializer->ResDesc.Height;
		desc.DepthOrArraySize = _Initializer->ResDesc.DepthOrArraySize;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.MipLevels = _Initializer->ResDesc.MipLevels;
		desc.Dimension = Glue::GetD3DResourceDimension(_Initializer->ResDesc.Dimension);
		desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;
	}

	const D3D12_RESOURCE_DESC* pDesc = nullptr;

	if (_Initializer->pResDesc)
	{
		pDesc = PCast<const D3D12_RESOURCE_DESC*>(_Initializer->pResDesc);
	}
	else
	{
		pDesc = &desc;
	}

	D3D12_CLEAR_VALUE ClearValue = {};
	D3D12_CLEAR_VALUE* pClearValue = nullptr;

	if (_Initializer->ClearValue != nullptr)
	{
		ClearValue = CD3DX12_CLEAR_VALUE(pDesc->Format, _Initializer->ClearValue);
		pClearValue = &ClearValue;
	}

	D3D_ERROR_CHECK(_Initializer->Device->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		pDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		pClearValue,
		IID_PPV_ARGS(&m_Resource)
	));

	m_ResourceName = _Initializer->Name;

	return true;
}

bool CTextureResource::CopyToVideoMemory(const FTextureInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(_Initializer->Device);

	CHECK_RESULT_FALSE(m_Resource);

	// レンダーターゲットのようなピクセル情報がないデータでも許容できるようにここを修正しておく
	if (!_Initializer->SourceData)
	{
		return true;
	}

	D3D_ERROR_CHECK(m_Resource->WriteToSubresource(
		0,
		nullptr,
		_Initializer->SourceData,
		_Initializer->RowPitch,
		_Initializer->SlicePitch
	));

	return true;
}
