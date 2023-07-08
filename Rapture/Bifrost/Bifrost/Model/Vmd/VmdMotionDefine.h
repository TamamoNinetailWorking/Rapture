#pragma once

#include <Atlantis/DirectX12/GlueMath.h>
#include <vector>

BIFROST_NAMESPACE_BEGIN

#if 0
struct FVmdMotionResourceData
{
	uint32 FrameNo = 0;
	ATLANTIS_NAMESPACE::Glue::FVector Quaternion = {};
};
#endif

struct FVmdMotionPerKeyFrame
{
	uint32 FrameNo = 0;
	ATLANTIS_NAMESPACE::Glue::FVector Quaternion = {};
};

BIFROST_NAMESPACE_END