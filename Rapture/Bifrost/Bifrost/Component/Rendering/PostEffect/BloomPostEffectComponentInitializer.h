#pragma once

#include <Bifrost/Component/ComponentInitializer.h>

BIFROST_NAMESPACE_BEGIN

struct FBloomPostEffectComponentInitializer : public FComponentInitializerBase
{
	Hash160 QuadPolygonPsoName = {};
	Hash160 BrightnessPsoName = {};
	Hash160 BlurPsoName = {};
	Hash160 FetchColorPsoName = {};

	uint32 Width = 0;
	uint32 Height = 0;
};

BIFROST_NAMESPACE_END