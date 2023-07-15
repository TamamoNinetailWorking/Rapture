#pragma once

//#include <eden/include/Resource/ResourceInitializer.h>
#include <Atlantis/Material/MaterialInterfaceInitializer.h>

//struct ID3D12Device;
ATLANTIS_NAMESPACE_BEGIN

class CDX12MainDevice;

ATLANTIS_NAMESPACE_END

BIFROST_NAMESPACE_BEGIN

struct FPmdMaterialData;
struct FPipelineStateObjectInitializer;

struct FPmdMaterialInitializer : public ATLANTIS_NAMESPACE::FMaterialInterfaceInitializerBase
{
	ATLANTIS_NAMESPACE::CDX12MainDevice* Device = nullptr;
	Hash160 Name = {};
	Hash160 ToonMapDirectory = {};
	const FPmdMaterialData* Materials = nullptr;
	uint32 MaterialNum = 0;

	FPipelineStateObjectInitializer* PsoInit = nullptr;
};

BIFROST_NAMESPACE_END