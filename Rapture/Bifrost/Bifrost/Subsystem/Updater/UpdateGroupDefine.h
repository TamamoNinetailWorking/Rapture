#pragma once

BIFROST_NAMESPACE_BEGIN

enum class EUpdateGroup : uint8
{
	UPDATE_GROUP_PRE_PHYSICS,

	UPDATE_GROUP_PHYSICS_DURING,

	UPDATE_GROUP_POST_PHYSICS,

	UPDATE_GROUP_NUM,
	UPDATE_GROUP_INVALID,
};

BIFROST_NAMESPACE_END