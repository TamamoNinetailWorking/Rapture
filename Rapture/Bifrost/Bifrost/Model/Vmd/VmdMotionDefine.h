#pragma once

#include <Atlantis/DirectX12/GlueMath.h>

BIFROST_NAMESPACE_BEGIN

struct FVmdMotionResourceData
{
	uint32 FrameNo = 0;
	ATLANTIS_NAMESPACE::Glue::FVector Quaternion = {};
};


BIFROST_NAMESPACE_END