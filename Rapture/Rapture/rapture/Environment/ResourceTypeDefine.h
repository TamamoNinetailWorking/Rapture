#pragma once

enum class EResourceManagementType : uint8
{
	RESOURCE_TYPE_TEXTURE = 0,
	RESOURCE_TYPE_MESH,
	RESOURCE_TYPE_SHADER,
	RESOURCE_TYPE_MATERIAL,
	RESOURCE_TYPE_PSO,
	RESOURCE_TYPE_MOTION,

	RESOURCE_TYPE_NUM,
};