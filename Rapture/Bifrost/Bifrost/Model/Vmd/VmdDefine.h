#pragma once

#include <Atlantis/DirectX12/GlueMath.h>

BIFROST_NAMESPACE_BEGIN

constexpr uint8 VmdBoneNameLength = 15;
constexpr uint8 VmdMotionInterpolateDataLength = 64;

struct FVmdMotionData
{
public:
	uint8 Name[VmdBoneNameLength] = {};
private:
	uint8 padding01 = {};
public:
	uint16 FrameNo = 0;
private:
	uint16 padding02 = {};
public:
	
	ATLANTIS_NAMESPACE::Glue::Vector3 Locate = {};

	ATLANTIS_NAMESPACE::Glue::Vector4 Quat = {};

	uint8 InterpolateData[VmdMotionInterpolateDataLength] = {};

};


constexpr uint8 VmdBlendShapeNameLength = 15;

struct FVmdBlendShapeData
{
public:
	uint8 Name[VmdBlendShapeNameLength] = {};
private:
	uint8 padding01 = {};
public:
	uint16 FlameNo = 0;
private:
	uint16 padding02 = {};
public:
	float Weight = 0.f;
};


constexpr uint8 VmdCameraInterpolateDataSize = 24;

struct FVmdCameraData
{
public:
	uint16 FlameNo = 0;
private:
	uint16 padding01 = {};
public:
	float Distance = 0.f;

	ATLANTIS_NAMESPACE::Glue::Vector3 Locate = {};

	ATLANTIS_NAMESPACE::Glue::Vector3 Rotate = {};

	uint8 Interpolate[VmdCameraInterpolateDataSize] = {};

	uint16 FieldOfView = 0;

	bool IsNotPerspective = true;

private:
	uint8 padding02 = {};
};


struct FVmdLightData
{
public:
	uint16 FlameNo = 0;
private:
	uint16 padding01 = {};
public:
	ATLANTIS_NAMESPACE::Glue::Vector3 Color = {};

	ATLANTIS_NAMESPACE::Glue::Vector3 Locate = {};
};


struct FVmdSelfShadowData
{
public:
	uint16 FlameNo = 0;
	uint8 Mode = 0;
private:
	uint8 padding01 = {};
public:
	float Distance = 0.f;
};


BIFROST_NAMESPACE_END