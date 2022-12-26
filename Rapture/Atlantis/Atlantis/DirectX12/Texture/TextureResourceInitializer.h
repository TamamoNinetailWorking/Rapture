#pragma once

#include <eden/include/Resource/ResourceInitializer.h>
#include <Atlantis/DirectX12/DirectXPaste.h>

ATLANTIS_NAMESPACE_BEGIN

struct FTextureInitializer : public EDENS_NAMESPACE::FResourceInitializerBase
{
	struct ID3D12Device* Device = nullptr;

	const void* SourceData = nullptr;

	uint32 Width = 0;
	uint32 Height = 0;

	uint32 RowPitch = 0;
	uint32 SlicePitch = 0;

	uint32 DepthOrArraySize = 1;
	uint32 MipLevels = 1;

	Glue::EDataFormat Format = {};
	Glue::EResourceDimension Dimension = {};
};

ATLANTIS_NAMESPACE_END