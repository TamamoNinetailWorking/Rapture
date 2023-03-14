#pragma once

#include <vector>

#include <Atlantis/DirectX12/GlueMath.h>

BIFROST_NAMESPACE_BEGIN

struct FPmdBoneNode
{
	uint32 BoneIndex = 0;
	ATLANTIS_NAMESPACE::Glue::Vector3 StartPos = {};
	ATLANTIS_NAMESPACE::Glue::Vector3 EndPos = {};

	using FBoneNodeList = std::vector<FPmdBoneNode*>;

	FBoneNodeList children = {};
};

BIFROST_NAMESPACE_END