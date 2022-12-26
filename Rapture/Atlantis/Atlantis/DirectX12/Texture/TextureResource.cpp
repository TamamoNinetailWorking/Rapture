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
	prop.Type = D3D12_HEAP_TYPE_CUSTOM;
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	prop.CreationNodeMask = 0;
	prop.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC desc = {};
	desc.Format = Glue::GetDXGIFormat(_Initializer->Format);
	desc.Width = _Initializer->Width;
	desc.Height = _Initializer->Height;
	desc.DepthOrArraySize = _Initializer->DepthOrArraySize;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.MipLevels = _Initializer->MipLevels;
	desc.Dimension = Glue::GetD3DResourceDimension(_Initializer->Dimension);
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	D3D_ERROR_CHECK(_Initializer->Device->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&m_Resource)
	));

	m_ResourceName = _Initializer->Name;

	return true;
}

bool CTextureResource::CopyToVideoMemory(const FTextureInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(_Initializer->Device);
	CHECK_RESULT_FALSE(_Initializer->SourceData);

	CHECK_RESULT_FALSE(m_Resource);

	D3D_ERROR_CHECK(m_Resource->WriteToSubresource(
		0,
		nullptr,
		_Initializer->SourceData,
		_Initializer->RowPitch,
		_Initializer->SlicePitch
	));

	return true;
}
