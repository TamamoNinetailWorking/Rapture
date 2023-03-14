#pragma once

BIFROST_NAMESPACE_BEGIN

struct FPmdBoneData;

struct FPmdSkeletonInitializer
{
	const FPmdBoneData* Bones = nullptr;
	uint32 BoneNum = 0;
};

BIFROST_NAMESPACE_END