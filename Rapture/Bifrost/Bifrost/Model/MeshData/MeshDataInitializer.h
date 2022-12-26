#pragma once

#include <eden/include/Resource/ResourceInitializer.h>

//ATLANTIS_NAMESPACE_BEGIN
BIFROST_NAMESPACE_BEGIN

struct FMeshResourceInitializer : public EDENS_NAMESPACE::FResourceInitializerBase
{
	struct ID3D12Device* Device = nullptr;

	const void* Vertices = nullptr;
	uint32 VerticesSize = 0;
	uint32 StrideBytes = 0;

	const uint16* Indices = nullptr;
	uint32 IndicesSize = 0;
};

BIFROST_NAMESPACE_END
//ATLANTIS_NAMESPACE_END