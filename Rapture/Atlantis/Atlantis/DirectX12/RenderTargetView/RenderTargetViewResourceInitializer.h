#pragma once

#include <eden/include/Resource/ResourceInitializer.h>

#include <Atlantis/DirectX12/DirectXPaste.h>

ATLANTIS_NAMESPACE_BEGIN

class CTextureResource;

struct FRenderTargetViewResourceInitializer : public EDENS_NAMESPACE::FResourceInitializerBase
{
	ID3D12Device* Device = nullptr;
	ID3D12Resource* ResPtr = nullptr;

	Glue::EDataFormat Format = {};
	Glue::ERTVDimension RTVDimension = {};
};

ATLANTIS_NAMESPACE_END