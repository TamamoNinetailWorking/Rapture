#pragma once

//#include <Atlantis/DirectX12/DirectXPaste.h>
#include <Atlantis/DirectX12/GlueMath.h>
#include <eden/include/Resource/ResourceManagerPreDefine.h>
#include <Atlantis/Material/MaterialInterfaceDefine.h>


BIFROST_NAMESPACE_BEGIN

// �R���X�^���g�o�b�t�@
struct FSceneData : public ATLANTIS_NAMESPACE::FMaterialGeometryBufferBase
{

	using Matrix = ATLANTIS_NAMESPACE::Glue::FMatrix;
	using Vec3 = ATLANTIS_NAMESPACE::Glue::Vector3;

	Matrix World = {};
	Matrix View = {};
	Matrix ViewProjection = {};
	Matrix WorldViewProjection = {};
	Vec3 EyePosition = {};
	float padding1 = 0.f;
	Vec3 LightPosition = {};
	float padding2 = 0.f;
	Vec3 LightColor = {};
	float padding3 = 0.f;

};

struct FMaterialConstantData
{
	ATLANTIS_NAMESPACE::Glue::Vector3 Diffuse = {};
	float Alpha = 0.f;
	ATLANTIS_NAMESPACE::Glue::Vector3 SpecularColor = {};
	float Specularity = 0.f;
	ATLANTIS_NAMESPACE::Glue::Vector3 AmbientColor = {};
	float AmbientAlpha = 0.f;
};

struct FMaterialResourceData
{
	EDENS_NAMESPACE::FResourceHandle ColorTexture = {};
	EDENS_NAMESPACE::FResourceHandle SphereTexture = {};
	EDENS_NAMESPACE::FResourceHandle SphereAddTexture = {};
	EDENS_NAMESPACE::FResourceHandle ToonResource = {};
};

BIFROST_NAMESPACE_END