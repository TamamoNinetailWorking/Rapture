#pragma once

#include <Bifrost/Resource/Motion/MotionResourceInitializer.h>

BIFROST_NAMESPACE_BEGIN

struct FVmdMotionData;

struct FVmdMotionResourceInitializer : BIFROST_NAMESPACE::FMotionResourceInitializerBase
{
	//Hash160 MotionName = {};
	const FVmdMotionData* Motions = nullptr;
	uint32 MotionNum = 0;
};

BIFROST_NAMESPACE_END