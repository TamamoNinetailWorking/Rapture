#pragma once

#include <eden/include/Resource/ResourceInitializer.h>
#include <Atlantis/DirectX12/DirectXPaste.h>

//struct ID3D12Device;
struct D3D12_ROOT_PARAMETER;
struct D3D12_STATIC_SAMPLER_DESC;
struct D3D12_GRAPHICS_PIPELINE_STATE_DESC;

ATLANTIS_NAMESPACE_BEGIN

struct FShaderBaseInitializer;
class CDX12MainDevice;

ATLANTIS_NAMESPACE_END



BIFROST_NAMESPACE_BEGIN

struct FPipelineStateObjectInitializer : public EDENS_NAMESPACE::FResourceInitializerBase
{
#define GLUE_NAMESPACE ATLANTIS_NAMESPACE::Glue

	//ID3D12Device* Device = nullptr;
	ATLANTIS_NAMESPACE::CDX12MainDevice* Device = nullptr;

	GLUE_NAMESPACE::ERootSignatureFlag Flag = GLUE_NAMESPACE::ERootSignatureFlag::ROOT_SIGNATURE_FLAG_NONE;
	
	D3D12_ROOT_PARAMETER* pParameters = nullptr;
	D3D12_STATIC_SAMPLER_DESC* pStaticSamplers = nullptr;
	uint32 NumParameters = 0;
	uint32 NumStaticSamplers = 0;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC* pPipelineDesc = nullptr;

//#define FShaderBaseInit ATLANTIS_NAMESPACE::FShaderBaseInitializer
	using FShaderBaseInit = ATLANTIS_NAMESPACE::FShaderBaseInitializer;

	const FShaderBaseInit* VertexShaderInit = {};
	const FShaderBaseInit* DomainShaderInit = {};
	const FShaderBaseInit* HullShaderInit = {};
	const FShaderBaseInit* GeometryShaderInit = {};
	const FShaderBaseInit* PixelShaderInit = {};

//#undef FShaderBaseInit

#undef GLUE_NAMESPACE

};

BIFROST_NAMESPACE_END