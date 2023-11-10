#pragma once

#include <Atlantis/Math/GlueMath.h>

BIFROST_NAMESPACE_BEGIN

enum class EBloomPostEffectRTVBufferIndex : uint8
{
	Result = 0,
	Bright,
	Blur1,
	Blur2,
	Blur3,
	Blur4,
	RTV_BUFFER_NUM,
};

struct FBloomPostEffectParameterBuffer
{
	float BrightThreshold = 0;
	int32 SampleCount = 0;
	ATLANTIS_NAMESPACE::Glue::Vector4 Offset[16] = {};
};

BIFROST_NAMESPACE_END