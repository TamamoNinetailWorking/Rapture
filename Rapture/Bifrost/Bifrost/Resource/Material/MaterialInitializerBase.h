#pragma once

#include <eden/include/Resource/ResourceInitializer.h>

BIFROST_NAMESPACE_BEGIN

enum EMaterialType : uint8
{
	MATERIAL_TYPE_PMD,


	MATERIAL_TYPE_NUM,
};

struct FMaterialInitializerBase : EDENS_NAMESPACE::FResourceInitializerBase
{
	EMaterialType Type = {};
};

BIFROST_NAMESPACE_END