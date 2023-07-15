#pragma once

#include <Atlantis/DirectX12/DirectXPaste.h>
#include <Atlantis/RHIProccessor/RHIRenderTargetDefine.h>

BIFROST_NAMESPACE_BEGIN

struct FRenderTargetInitializer
{
	Hash160 Name = {};

	const void* pResDesc = nullptr;
	const float* ClearValue = ATLANTIS_NAMESPACE::RenderTargetClearColor;

	ATLANTIS_NAMESPACE::Glue::EHeapType HeapType = ATLANTIS_NAMESPACE::Glue::EHeapType::DEFAULT;

	ATLANTIS_NAMESPACE::Glue::ERTVDimension Dimension = ATLANTIS_NAMESPACE::Glue::ERTVDimension::RTV_DIMENSION_TEXTURE2D;
};

BIFROST_NAMESPACE_END