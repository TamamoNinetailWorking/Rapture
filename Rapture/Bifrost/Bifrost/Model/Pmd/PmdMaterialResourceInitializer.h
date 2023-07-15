#pragma once

#include <Bifrost/Resource/Material/MaterialInitializerBase.h>

//struct ID3D12Device;
ATLANTIS_NAMESPACE_BEGIN

class CDX12MainDevice;

ATLANTIS_NAMESPACE_END


BIFROST_NAMESPACE_BEGIN

struct FPmdMaterialData;

struct FPmdMaterialResourceInitializer : public BIFROST_NAMESPACE::FMaterialInitializerBase
{
	ATLANTIS_NAMESPACE::CDX12MainDevice* Device = nullptr;
	Hash160 ToonMapDirectory = {};
	Hash160 ModelName = {};
	const FPmdMaterialData* Material = nullptr;
};

BIFROST_NAMESPACE_END