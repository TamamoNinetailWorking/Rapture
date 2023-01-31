#pragma once

//#include <Atlantis/DirectX12/DirectXPaste.h>
#include <Atlantis/DirectX12/GlueMath.h>

BIFROST_NAMESPACE_BEGIN

#define ATLANTIS_GLUE ATLANTIS_NAMESPACE::Glue

static const uint8 BoneNoLength = 2;
static const uint8 TexFilePathLength = 20;

struct FPmdVertex
{
	ATLANTIS_GLUE::Vector3 Pos = {};
	ATLANTIS_GLUE::Vector3 Normal = {};
	ATLANTIS_GLUE::Vector2 UV = {};
	uint16 BoneNo[BoneNoLength] = {};
	uint8 BoneWeight = {};
	uint8 EdgeFlag = {};
	uint16 padding = {};	// データの詰め方のせいで4Byteアラインだとパディングが発生する
};

struct FPmdMaterialData
{
	ATLANTIS_GLUE::Vector3 Diffuse = {};
	float Alpha = 0.f;
	float Specularity = 0.f;
	ATLANTIS_GLUE::Vector3 SpecularColor = {};
	ATLANTIS_GLUE::Vector3 AmbientColor = {};
	uint8 ToonIndex = {};
	uint8 EdgeFlag = {};
	// padding 16bit
	uint32 IndiciesNum = 0;
	char TexFilePath[TexFilePathLength] = {};
};

#undef ATLANTIS_GLUE

BIFROST_NAMESPACE_END