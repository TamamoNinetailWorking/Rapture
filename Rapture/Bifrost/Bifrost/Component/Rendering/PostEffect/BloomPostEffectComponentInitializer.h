#pragma once

#include <Bifrost/Component/ComponentInitializer.h>

BIFROST_NAMESPACE_BEGIN

struct FBloomPostEffectComponentInitializer : public FComponentInitializerBase
{
	Hash160 QuadPolygonPsoName = {};
};

BIFROST_NAMESPACE_END