#pragma once

#include <Bifrost/Actor/ActorInitializerBase.h>

BIFROST_NAMESPACE_BEGIN

struct FPostEffectActorInitializer : public FActorInitializerBase
{
	Hash160 QuadPolygonPsoName = {};
	Hash160 BrightnessPsoName = {};
	Hash160 BlurPsoName = {};
	Hash160 FetchColorPsoName = {};
};

BIFROST_NAMESPACE_END