#pragma once

#include <eden/include/Resource/ResourceInitializer.h>

BIFROST_NAMESPACE_BEGIN

enum class EMotionType : uint8
{
	MOTION_TYPE_VMD,

	MOTION_TYPE_NUM,
};

struct FMotionResourceInitializerBase : EDENS_NAMESPACE::FResourceInitializerBase
{
	EMotionType Type = {};
};


BIFROST_NAMESPACE_END