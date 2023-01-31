﻿#pragma once

#include <d3d12.h>

#define USING_GLUE using namespace ATLANTIS_NAMESPACE::Glue;

ATLANTIS_NAMESPACE_BEGIN

// DirectX特有のものから予め定義したものに置き換えていく
namespace Glue
{

	enum ECommandListType : uint8
	{
		COMMAND_LIST_TYPE_DIRECT = 0,
		COMMAND_LIST_TYPE_BUNDLE = 1,
		COMMAND_LIST_TYPE_COMPUTE = 2,
		COMMAND_LIST_TYPE_COPY = 3,
		COMMAND_LIST_TYPE_VIDEO_DECODE = 4,
		COMMAND_LIST_TYPE_VIDEO_PROCESS = 5,
		COMMAND_LIST_TYPE_VIDEO_ENCODE = 6
	};

	const D3D12_COMMAND_LIST_TYPE GetD3DCommandListType(ECommandListType _ListType);


	enum ECommandQueueFlag : uint8
	{
		COMMAND_QUEUE_FLAG_NONE = 0,
		COMMAND_QUEUE_FLAG_DISABLE_GPU_TIMEOUT = 0x1
	};

	const D3D12_COMMAND_QUEUE_FLAGS GetD3DCommandQueueFlag(ECommandQueueFlag _Flag);


	enum ECommandQueuePriority : uint32
	{
		COMMAND_QUEUE_PRIORITY_NORMAL = 0,
		COMMAND_QUEUE_PRIORITY_HIGH = 100,
		COMMAND_QUEUE_PRIORITY_GLOBAL_REALTIME = 10000
	};

	const D3D12_COMMAND_QUEUE_PRIORITY GetD3DCommandQueuePriority(ECommandQueuePriority _Priority);


	enum ERootSignatureFlag : uint16
	{
		ROOT_SIGNATURE_FLAG_NONE = 0,
		ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT = 0x1,
		ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS = 0x2,
		ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS = 0x4,
		ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS = 0x8,
		ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS = 0x10,
		ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS = 0x20,
		ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT = 0x40,
		ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE = 0x80,
		ROOT_SIGNATURE_FLAG_DENY_AMPLIFICATION_SHADER_ROOT_ACCESS = 0x100,
		ROOT_SIGNATURE_FLAG_DENY_MESH_SHADER_ROOT_ACCESS = 0x200
	};

	const D3D12_ROOT_SIGNATURE_FLAGS GetD3DRootSignatureFlag(ERootSignatureFlag _Flag);


	enum EResourceBarrierType : uint8
	{
		BARRIER_TYPE_TRANSITION = 0,
		BARRIER_TYPE_ALIASING = (BARRIER_TYPE_TRANSITION + 1),
		BARRIER_TYPE_UAV = (BARRIER_TYPE_ALIASING + 1)
	};

	const D3D12_RESOURCE_BARRIER_TYPE GetD3DResourceBarrierType(EResourceBarrierType _Type);


	enum EResourceState : uint32
	{
		RESOURCE_STATE_COMMON = 0,
		RESOURCE_STATE_PRESENT = RESOURCE_STATE_COMMON,

		RESOURCE_STATE_RENDER_TARGET = 0x4,
	};

	const D3D12_RESOURCE_STATES GetD3DResourceState(EResourceState _state);


	enum EResourceDimension : uint8
	{
		RESOURCE_DIMENSION_UNKNOWN = 0,
		RESOURCE_DIMENSION_BUFFER = 1,
		RESOURCE_DIMENSION_TEXTURE1D = 2,
		RESOURCE_DIMENSION_TEXTURE2D = 3,
		RESOURCE_DIMENSION_TEXTURE3D = 4
	};

	const D3D12_RESOURCE_DIMENSION GetD3DResourceDimension(EResourceDimension _dimension);

    enum ERTVDimension : uint8
    {
        RTV_DIMENSION_UNKNOWN = 0,
        RTV_DIMENSION_BUFFER = 1,
        RTV_DIMENSION_TEXTURE1D = 2,
        RTV_DIMENSION_TEXTURE1DARRAY = 3,
        RTV_DIMENSION_TEXTURE2D = 4,
        RTV_DIMENSION_TEXTURE2DARRAY = 5,
        RTV_DIMENSION_TEXTURE2DMS = 6,
        RTV_DIMENSION_TEXTURE2DMSARRAY = 7,
        RTV_DIMENSION_TEXTURE3D = 8
    };

    const D3D12_RTV_DIMENSION GetD3DRTVDimension(ERTVDimension _Dimension);

	enum EDataFormat : uint8
	{
        FORMAT_UNKNOWN = 0,
        FORMAT_R32G32B32A32_TYPELESS = 1,
        FORMAT_R32G32B32A32_FLOAT = 2,
        FORMAT_R32G32B32A32_UINT = 3,
        FORMAT_R32G32B32A32_SINT = 4,
        FORMAT_R32G32B32_TYPELESS = 5,
        FORMAT_R32G32B32_FLOAT = 6,
        FORMAT_R32G32B32_UINT = 7,
        FORMAT_R32G32B32_SINT = 8,
        FORMAT_R16G16B16A16_TYPELESS = 9,
        FORMAT_R16G16B16A16_FLOAT = 10,
        FORMAT_R16G16B16A16_UNORM = 11,
        FORMAT_R16G16B16A16_UINT = 12,
        FORMAT_R16G16B16A16_SNORM = 13,
        FORMAT_R16G16B16A16_SINT = 14,
        FORMAT_R32G32_TYPELESS = 15,
        FORMAT_R32G32_FLOAT = 16,
        FORMAT_R32G32_UINT = 17,
        FORMAT_R32G32_SINT = 18,
        FORMAT_R32G8X24_TYPELESS = 19,
        FORMAT_D32_FLOAT_S8X24_UINT = 20,
        FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
        FORMAT_X32_TYPELESS_G8X24_UINT = 22,
        FORMAT_R10G10B10A2_TYPELESS = 23,
        FORMAT_R10G10B10A2_UNORM = 24,
        FORMAT_R10G10B10A2_UINT = 25,
        FORMAT_R11G11B10_FLOAT = 26,
        FORMAT_R8G8B8A8_TYPELESS = 27,
        FORMAT_R8G8B8A8_UNORM = 28,
        FORMAT_R8G8B8A8_UNORM_SRGB = 29,
        FORMAT_R8G8B8A8_UINT = 30,
        FORMAT_R8G8B8A8_SNORM = 31,
        FORMAT_R8G8B8A8_SINT = 32,
        FORMAT_R16G16_TYPELESS = 33,
        FORMAT_R16G16_FLOAT = 34,
        FORMAT_R16G16_UNORM = 35,
        FORMAT_R16G16_UINT = 36,
        FORMAT_R16G16_SNORM = 37,
        FORMAT_R16G16_SINT = 38,
        FORMAT_R32_TYPELESS = 39,
        FORMAT_D32_FLOAT = 40,
        FORMAT_R32_FLOAT = 41,
        FORMAT_R32_UINT = 42,
        FORMAT_R32_SINT = 43,
        FORMAT_R24G8_TYPELESS = 44,
        FORMAT_D24_UNORM_S8_UINT = 45,
        FORMAT_R24_UNORM_X8_TYPELESS = 46,
        FORMAT_X24_TYPELESS_G8_UINT = 47,
        FORMAT_R8G8_TYPELESS = 48,
        FORMAT_R8G8_UNORM = 49,
        FORMAT_R8G8_UINT = 50,
        FORMAT_R8G8_SNORM = 51,
        FORMAT_R8G8_SINT = 52,
        FORMAT_R16_TYPELESS = 53,
        FORMAT_R16_FLOAT = 54,
        FORMAT_D16_UNORM = 55,
        FORMAT_R16_UNORM = 56,
        FORMAT_R16_UINT = 57,
        FORMAT_R16_SNORM = 58,
        FORMAT_R16_SINT = 59,
        FORMAT_R8_TYPELESS = 60,
        FORMAT_R8_UNORM = 61,
        FORMAT_R8_UINT = 62,
        FORMAT_R8_SNORM = 63,
        FORMAT_R8_SINT = 64,
        FORMAT_A8_UNORM = 65,
        FORMAT_R1_UNORM = 66,
        FORMAT_R9G9B9E5_SHAREDEXP = 67,
        FORMAT_R8G8_B8G8_UNORM = 68,
        FORMAT_G8R8_G8B8_UNORM = 69,
        FORMAT_BC1_TYPELESS = 70,
        FORMAT_BC1_UNORM = 71,
        FORMAT_BC1_UNORM_SRGB = 72,
        FORMAT_BC2_TYPELESS = 73,
        FORMAT_BC2_UNORM = 74,
        FORMAT_BC2_UNORM_SRGB = 75,
        FORMAT_BC3_TYPELESS = 76,
        FORMAT_BC3_UNORM = 77,
        FORMAT_BC3_UNORM_SRGB = 78,
        FORMAT_BC4_TYPELESS = 79,
        FORMAT_BC4_UNORM = 80,
        FORMAT_BC4_SNORM = 81,
        FORMAT_BC5_TYPELESS = 82,
        FORMAT_BC5_UNORM = 83,
        FORMAT_BC5_SNORM = 84,
        FORMAT_B5G6R5_UNORM = 85,
        FORMAT_B5G5R5A1_UNORM = 86,
        FORMAT_B8G8R8A8_UNORM = 87,
        FORMAT_B8G8R8X8_UNORM = 88,
        FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
        FORMAT_B8G8R8A8_TYPELESS = 90,
        FORMAT_B8G8R8A8_UNORM_SRGB = 91,
        FORMAT_B8G8R8X8_TYPELESS = 92,
        FORMAT_B8G8R8X8_UNORM_SRGB = 93,
        FORMAT_BC6H_TYPELESS = 94,
        FORMAT_BC6H_UF16 = 95,
        FORMAT_BC6H_SF16 = 96,
        FORMAT_BC7_TYPELESS = 97,
        FORMAT_BC7_UNORM = 98,
        FORMAT_BC7_UNORM_SRGB = 99,
        FORMAT_AYUV = 100,
        FORMAT_Y410 = 101,
        FORMAT_Y416 = 102,
        FORMAT_NV12 = 103,
        FORMAT_P010 = 104,
        FORMAT_P016 = 105,
        FORMAT_420_OPAQUE = 106,
        FORMAT_YUY2 = 107,
        FORMAT_Y210 = 108,
        FORMAT_Y216 = 109,
        FORMAT_NV11 = 110,
        FORMAT_AI44 = 111,
        FORMAT_IA44 = 112,
        FORMAT_P8 = 113,
        FORMAT_A8P8 = 114,
        FORMAT_B4G4R4A4_UNORM = 115,

        FORMAT_P208 = 130,
        FORMAT_V208 = 131,
        FORMAT_V408 = 132,


        FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE = 189,
        FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE = 190,


        FORMAT_FORCE_UINT = 0xff

	};

    const DXGI_FORMAT GetDXGIFormat(EDataFormat _format);


    enum EDescriptorHeapRangeType
    {
        RANGE_TYPE_SRV = 0,
        RANGE_TYPE_UAV,
        RANGE_TYPE_CBV,
        RANGE_TYPE_SAMPLER
    };

    const D3D12_DESCRIPTOR_RANGE_TYPE GetRangeType(EDescriptorHeapRangeType _Type);


    enum EShaderVisibility
    {
        SHADER_VISIBILITY_ALL = 0,
        SHADER_VISIBILITY_VERTEX = 1,
        SHADER_VISIBILITY_HULL = 2,
        SHADER_VISIBILITY_DOMAIN = 3,
        SHADER_VISIBILITY_GEOMETRY = 4,
        SHADER_VISIBILITY_PIXEL = 5,
        SHADER_VISIBILITY_AMPLIFICATION = 6,
        SHADER_VISIBILITY_MESH = 7
    };

    const D3D12_SHADER_VISIBILITY GetShaderVisibility(EShaderVisibility _Visibility);
};


ATLANTIS_NAMESPACE_END