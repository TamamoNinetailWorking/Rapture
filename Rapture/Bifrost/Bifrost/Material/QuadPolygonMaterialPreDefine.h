#pragma once

#include <Atlantis/DirectX12/GlueMath.h>
#include <Atlantis/Material/MaterialInterfaceDefine.h>

BIFROST_NAMESPACE_BEGIN

struct FQuadPolygonSceneData : public ATLANTIS_NAMESPACE::FMaterialGeometryBufferBase
{
	using Matrix = ATLANTIS_NAMESPACE::Glue::FMatrix;

	struct Data
	{
		Matrix WorldViewProjection = {};
	};

	ObjectPtr(Data) pData = nullptr;
};

BIFROST_NAMESPACE_END