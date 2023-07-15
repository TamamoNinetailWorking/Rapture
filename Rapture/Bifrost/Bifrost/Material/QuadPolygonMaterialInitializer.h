#pragma once

#include <Atlantis/Material/MaterialInterfaceInitializer.h>

BIFROST_NAMESPACE_BEGIN

struct FQuadPolygonMaterialInitializer : public ATLANTIS_NAMESPACE::FMaterialInterfaceInitializerBase
{
	Hash160 PsoName = {};
};

BIFROST_NAMESPACE_END