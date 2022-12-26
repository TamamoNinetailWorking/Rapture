#pragma once

#include "ShaderBaseInitializer.h"

ATLANTIS_NAMESPACE_BEGIN

struct FVertexShaderInitializer : public FShaderBaseInitializer
{
public:
	struct ID3D12Device* Device = nullptr;
};

struct FDomainShaderInitializer : public FShaderBaseInitializer
{

};

struct FHullShaderInitializer : public FShaderBaseInitializer
{

};

struct FGeometryShaderInitializer : public FShaderBaseInitializer
{

};

struct FPixelShaderInitializer : public FShaderBaseInitializer
{

};

ATLANTIS_NAMESPACE_END