#pragma once

#include <Bifrost/Actor/ActorInitializerBase.h>

BIFROST_NAMESPACE_BEGIN

struct FPostEffectActorInitializer : public FActorInitializerBase
{
	Hash160 QuadPolygonPsoName = {};
};

BIFROST_NAMESPACE_END