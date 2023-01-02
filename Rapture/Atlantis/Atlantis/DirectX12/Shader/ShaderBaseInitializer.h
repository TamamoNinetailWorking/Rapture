#pragma once

#include <eden/include/Resource/ResourceInitializer.h>

ATLANTIS_NAMESPACE_BEGIN

enum EShaderType : uint8
{
	SHADER_TYPE_VERTEX,
	SHADER_TYPE_DOMAIN,
	SHADER_TYPE_HULL,
	SHADER_TYPE_GEOMETRY,
	SHADER_TYPE_PIXEL,

	SHADER_TYPE_NUM,
};

struct FShaderBaseInitializer : public EDENS_NAMESPACE::FResourceInitializerBase
{
public:

	const void* Data = nullptr;
	uint32 Size = 0;

protected:
	friend class CShaderBase;

	Hash160 TargetHash = {};

public:

	EShaderType Type = {};
};

ATLANTIS_NAMESPACE_END