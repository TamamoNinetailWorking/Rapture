#pragma once

#include <eden/include/Resource/ResourceInitializer.h>

struct ID3D12Device;

BIFROST_NAMESPACE_BEGIN

struct FPmdMaterialData;

struct FPmdMaterialInitializer : public EDENS_NAMESPACE::FResourceInitializerBase
{
	ID3D12Device* Device = nullptr;
	Hash160 ToonMapDirectory = {};
	const FPmdMaterialData* Materials = nullptr;
	uint32 MaterialNum = 0;
};

BIFROST_NAMESPACE_END