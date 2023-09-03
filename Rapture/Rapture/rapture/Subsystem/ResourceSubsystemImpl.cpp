#include "ResourceSubsystemImpl.h"
#include <Bifrost/Subsystem/Resource/ResourceManagementSubsystem.h>
#include <eden/include/Resource/ResourceManager.h>

#include <Atlantis/DirectX12/MainDevice/MainDevice.h>

#include <Atlantis/DirectX12/DirectXHelper/d3dx12.h>

#include <eden/include/Resource/ResourceManagerPreDefine.h>
#include <Bifrost/Resource/DefaultResourceDefine.h>
#include <Atlantis/DirectX12/DirectX12BaseDefine.h>
#include <Atlantis/DirectX12/Texture/TextureResourceInitializer.h>

#include <eden/include/gadget/FileLoader/FileLoader.h>

#include <rapture/Environment/PsoNameDefine.h>
#include <Bifrost/Resource/PSO/PipelineStateObjectInitializer.h>
#include <Atlantis/DirectX12/Shader/GraphicsShaderInitializer.h>

USING_BIFROST;
USING_ATLANTIS;
EDENS_NAMESPACE_USING;

using namespace std;

#define SUBSYTEM_IS_NULL() \
	if(m_Subsystem == nullptr) { return nullptr; } \

struct FPipelineStateInitializerParameters
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC* pPipelineStateDesc = nullptr;
	D3D12_ROOT_PARAMETER* pRootParam = nullptr;
	uint32 RootParamNum = 0;
	D3D12_STATIC_SAMPLER_DESC* pStaticSamplerDesc = nullptr;
	uint32 SamplerParamNum = 0;
};

const IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetTextureResourceManager() const
{
	return GetManagerFromIndex(EResourceManagementType::RESOURCE_TYPE_TEXTURE);
}

IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetTextureResourceManagerEdit()
{
	return GetManagerFromIndexEdit(EResourceManagementType::RESOURCE_TYPE_TEXTURE);
}

const IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetMeshResourceManager() const
{
	return GetManagerFromIndex(EResourceManagementType::RESOURCE_TYPE_MESH);
}

IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetMeshResourceManagerEdit()
{
	return GetManagerFromIndexEdit(EResourceManagementType::RESOURCE_TYPE_MESH);
}

const IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetShaderResourceManager() const
{
	return GetManagerFromIndex(EResourceManagementType::RESOURCE_TYPE_SHADER);
}

IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetShaderResourceManagerEdit()
{
	return GetManagerFromIndexEdit(EResourceManagementType::RESOURCE_TYPE_SHADER);
}

const IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetRenderTargetViewResourceManager() const
{
	return GetManagerFromIndex(EResourceManagementType::RESOURCE_TYPE_RENDER_TARGET);
}

IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetRenderTargetViewResourceManagerEdit()
{
	return GetManagerFromIndexEdit(EResourceManagementType::RESOURCE_TYPE_RENDER_TARGET);
}

const IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetMaterialResourceManager() const
{
	return GetManagerFromIndex(EResourceManagementType::RESOURCE_TYPE_MATERIAL);
}

IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetMaterialResourceManagerEdit()
{
	return GetManagerFromIndexEdit(EResourceManagementType::RESOURCE_TYPE_MATERIAL);
}

const IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetPipelineStateObjectManager() const
{
	return GetManagerFromIndex(EResourceManagementType::RESOURCE_TYPE_PSO);
}

IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetPipelineStateObjectManagerEdit()
{
	return GetManagerFromIndexEdit(EResourceManagementType::RESOURCE_TYPE_PSO);
}

const IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetMotionResourceManager() const
{
	return GetManagerFromIndex(EResourceManagementType::RESOURCE_TYPE_MOTION);
}

IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetMotionResourceManagerEdit()
{
	return GetManagerFromIndexEdit(EResourceManagementType::RESOURCE_TYPE_MOTION);
}

void CResourceSubsystemImpl::Release()
{
	DeleteDefaultTextureResource();
	DeletePreDefPso();
	m_Subsystem = nullptr;
}

void CResourceSubsystemImpl::SetSubsystem(CResourceManagementSubsystem* _Subsystem)
{
	m_Subsystem = _Subsystem;
}

void CResourceSubsystemImpl::DeleteDefaultTextureResource()
{
	ManagerPtr manager = GetTextureResourceManager();
	if (manager == nullptr) { return; }

	manager->DeleteResource(DefaultResource::GrayGradationTextureResource);
	manager->DeleteResource(DefaultResource::BlackTextureResource);
	manager->DeleteResource(DefaultResource::WhiteTextureResource);
}

void CResourceSubsystemImpl::DeletePreDefPso()
{
	ManagerPtr manager = GetPipelineStateObjectManager();
	if (manager == nullptr) { return; }

	manager->DeleteResource(PsoName::PmdRenderingPso);
}

IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetManagerFromIndexEdit(EResourceManagementType _Type)
{
	SUBSYTEM_IS_NULL();
	uint32 index = UNumCast(_Type);
	return GetManager(index);
}

const IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetManagerFromIndex(EResourceManagementType _Type) const
{
	SUBSYTEM_IS_NULL();
	uint32 index = UNumCast(_Type);
	return GetManager(index);
}

bool CResourceSubsystemImpl::CreateDefaultTextureResource(const CDX12MainDevice* _Device)
{
	ManagerPtr manager = GetTextureResourceManager();
	if (manager == nullptr) { return false; }

	FResourceHandle handle = manager->GetInvalidHandle();

	FTextureInitializer initializer = {};
	{
		initializer.Device = _Device->GetDevice();
		initializer.ResDesc.Format = Glue::EDataFormat::FORMAT_R8G8B8A8_UNORM;
		initializer.ResDesc.Width = 4;
		initializer.ResDesc.Height = 4;
		initializer.ResDesc.DepthOrArraySize = 1;
		initializer.ResDesc.Dimension = Glue::EResourceDimension::RESOURCE_DIMENSION_TEXTURE2D;
		initializer.RowPitch = initializer.SlicePitch * initializer.ResDesc.Height;
		initializer.SlicePitch = initializer.ResDesc.Width * sizeof(uint8) * 4;
	}

	// White
	{
		vector<uint8> whiteData(4 * 4 * 4);
		fill(whiteData.begin(), whiteData.end(), 0xff);

		initializer.SourceData = whiteData.data();

		initializer.Name = DefaultResource::WhiteTextureResource;
		handle = manager->SearchCreateResource(&initializer);

		if(manager->IsInvalidHandle(handle))
		{
			return false;
		}
	}

	// Black
	{
		vector<uint8> blackData(4 * 4 * 4);
		fill(blackData.begin(), blackData.end(), 0x00);

		initializer.SourceData = blackData.data();

		initializer.Name = DefaultResource::BlackTextureResource;
		handle = manager->SearchCreateResource(&initializer);

		if (manager->IsInvalidHandle(handle))
		{
			return false;
		}
	}

	// GrayGradation
	{
		initializer.ResDesc.Height = 256;
		initializer.RowPitch = initializer.ResDesc.Width * sizeof(uint32);

		vector<uint32> grayData(4 * 256);
		uint32 color = 0xff;
		for (auto itr = grayData.begin(); itr != grayData.end(); itr += 4)
		{
			auto col = (0xff << 24) | RGB(color, color, color);
			fill(itr, itr + 4, col);
			--color;
		}

		initializer.SlicePitch = SCast<uint32>(grayData.size() * sizeof(uint32));
		initializer.SourceData = grayData.data();

		initializer.Name = DefaultResource::GrayGradationTextureResource;
		handle = manager->SearchCreateResource(&initializer);

		if (manager->IsInvalidHandle(handle))
		{
			return false;
		}
	}

	return true;
}

bool CResourceSubsystemImpl::CreatePmdPipelineStateObject(ATLANTIS_NAMESPACE::CDX12MainDevice* _Device, const ATLANTIS_NAMESPACE::FShaderNameBlock& _ShaderName)
{
	FPipelineStateObjectInitializer initializer = {};

	D3D12_DESCRIPTOR_RANGE descTableRange[3] = {};
	D3D12_ROOT_PARAMETER rootParam[2] = {};
	D3D12_STATIC_SAMPLER_DESC samplerDesc[2] = {};
	{
		// ディスクリプタ―テーブル << シェーダーで扱うレジスタの設定

		descTableRange[0].NumDescriptors = 1;
		descTableRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		descTableRange[0].BaseShaderRegister = 0;
		descTableRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		descTableRange[1].NumDescriptors = 1;
		descTableRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		descTableRange[1].BaseShaderRegister = 1;
		descTableRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		// テクスチャ用テーブル
		descTableRange[2].NumDescriptors = 4; //(普通のテクスチャとsphとspaとtoon)
		descTableRange[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descTableRange[2].BaseShaderRegister = 0;
		descTableRange[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


		rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		rootParam[0].DescriptorTable.pDescriptorRanges = &descTableRange[0];
		rootParam[0].DescriptorTable.NumDescriptorRanges = 1;

		rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParam[1].DescriptorTable.pDescriptorRanges = &descTableRange[1];
		rootParam[1].DescriptorTable.NumDescriptorRanges = 2;


		samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc[0].BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		samplerDesc[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc[0].MaxLOD = D3D12_FLOAT32_MAX;
		samplerDesc[0].MinLOD = 0.0f;
		samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		samplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		samplerDesc[0].ShaderRegister = 0;

		samplerDesc[1] = samplerDesc[0];
		samplerDesc[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerDesc[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerDesc[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerDesc[1].ShaderRegister = 1;


	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
	{
		desc.BlendState.AlphaToCoverageEnable = true;
		desc.BlendState.IndependentBlendEnable = false;// こいつがfalseならRenderTargetの設定は最初の1つだけでOKになる Independent => 個別に


		D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
		renderTargetBlendDesc.BlendEnable = false;
		renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		renderTargetBlendDesc.LogicOpEnable = false;

		desc.BlendState.RenderTarget[0] = renderTargetBlendDesc;


		desc.RasterizerState.MultisampleEnable = false;
		desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		//desc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		desc.RasterizerState.DepthClipEnable = true;

		desc.RasterizerState.FrontCounterClockwise = false;
		desc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		desc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		desc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		desc.RasterizerState.AntialiasedLineEnable = false;
		desc.RasterizerState.ForcedSampleCount = 0;
		desc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;


		desc.DepthStencilState.DepthEnable = true;
		desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // 小さい方（近い方）を採用
		desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		desc.DepthStencilState.StencilEnable = false;

		desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		desc.NumRenderTargets = 1;
		desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	}

	CFileLoader vsLoader = {};
	CFileLoader hsLoader = {};
	CFileLoader dsLoader = {};
	CFileLoader gsLoader = {};
	CFileLoader psLoader = {};

	FVertexShaderInitializer vsInit = {};
	FHullShaderInitializer hsInit = {};
	FDomainShaderInitializer dsInit = {};
	FGeometryShaderInitializer gsInit = {};
	FPixelShaderInitializer psInit = {};
	{

		{
			vsLoader.FileLoad(_ShaderName.VS.File.c_str());

			vsInit.Data = vsLoader.GetData();
			vsInit.Size = vsLoader.GetSize();
			vsInit.Name = _ShaderName.VS.GetShaderName();
			vsInit.Device = _Device->GetDevice();
			vsInit.Type = EShaderType::SHADER_TYPE_VERTEX;
		}

		{
			hsLoader.FileLoad(_ShaderName.HS.File.c_str());

			hsInit.Data = hsLoader.GetData();
			hsInit.Size = hsLoader.GetSize();
			hsInit.Name = _ShaderName.HS.GetShaderName();
			hsInit.Type = EShaderType::SHADER_TYPE_HULL;
		}

		{
			dsLoader.FileLoad(_ShaderName.DS.File.c_str());

			dsInit.Data = dsLoader.GetData();
			dsInit.Size = dsLoader.GetSize();
			dsInit.Name = _ShaderName.DS.GetShaderName();
			dsInit.Type = EShaderType::SHADER_TYPE_DOMAIN;
		}

		{
			gsLoader.FileLoad(_ShaderName.GS.File.c_str());

			gsInit.Data = gsLoader.GetData();
			gsInit.Size = gsLoader.GetSize();
			gsInit.Name = _ShaderName.GS.GetShaderName();
			gsInit.Type = EShaderType::SHADER_TYPE_GEOMETRY;
		}

		{
			psLoader.FileLoad(_ShaderName.PS.File.c_str());
			
			psInit.Data = psLoader.GetData();
			psInit.Size = psLoader.GetSize();
			psInit.Name = _ShaderName.PS.GetShaderName();
			psInit.Type = EShaderType::SHADER_TYPE_PIXEL;
		}


	}

	{
		using String = std::string;
		String name = {};
		if(vsInit.Name != INVALID_HASH) name += RHash160(vsInit.Name);
		if(hsInit.Name != INVALID_HASH) name += RHash160(hsInit.Name);
		if(dsInit.Name != INVALID_HASH) name += RHash160(dsInit.Name);
		if(gsInit.Name != INVALID_HASH) name += RHash160(gsInit.Name);
		if(psInit.Name != INVALID_HASH) name += RHash160(psInit.Name);

		initializer.Name = CHash160(name);
	}

	initializer.Device = _Device;
	initializer.Flag = Glue::ERootSignatureFlag::ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	initializer.pParameters = rootParam;
	initializer.NumParameters = 2;
	initializer.pStaticSamplers = samplerDesc;
	initializer.NumStaticSamplers = 2;

	initializer.pPipelineDesc = &desc;

	initializer.VertexShaderInit = &vsInit;
	initializer.HullShaderInit = &hsInit;
	initializer.DomainShaderInit = &dsInit;
	initializer.GeometryShaderInit = &gsInit;
	initializer.PixelShaderInit = &psInit;

	bool result = CreateRenderPipelineStateObject(&initializer);

	if (result)
	{
		PsoName::PmdRenderingPso = initializer.Name;
	}

	vsLoader.ResetData();
	hsLoader.ResetData();
	dsLoader.ResetData();
	gsLoader.ResetData();
	psLoader.ResetData();

	return result;
}

bool CResourceSubsystemImpl::CreateQuadPolygonPipelineStateObject(ATLANTIS_NAMESPACE::CDX12MainDevice* _Device, const ATLANTIS_NAMESPACE::FShaderNameBlock& _ShaderName)
{
	FPipelineStateObjectInitializer initializer = {};

	bool result = false;
	
	D3D12_GRAPHICS_PIPELINE_STATE_DESC Desc = {};
	{
		Desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		Desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		Desc.NumRenderTargets = 1;
		Desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		Desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

		Desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		Desc.SampleDesc.Count = 1;
		Desc.SampleDesc.Quality = 0;

		Desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	}

	D3D12_DESCRIPTOR_RANGE Range = {};
	{
		Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		Range.BaseShaderRegister = 0;
		Range.NumDescriptors = 1;
	}

	D3D12_ROOT_PARAMETER RootParam = {};
	{
		RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		RootParam.DescriptorTable.NumDescriptorRanges = 1;
		RootParam.DescriptorTable.pDescriptorRanges = &Range;
		RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	}

	D3D12_STATIC_SAMPLER_DESC Sampler = {};
	{
		Sampler = CD3DX12_STATIC_SAMPLER_DESC(0);
	}

	CFileLoader vsLoader = {};
	CFileLoader hsLoader = {};
	CFileLoader dsLoader = {};
	CFileLoader gsLoader = {};
	CFileLoader psLoader = {};

	FVertexShaderInitializer vsInit = {};
	FHullShaderInitializer hsInit = {};
	FDomainShaderInitializer dsInit = {};
	FGeometryShaderInitializer gsInit = {};
	FPixelShaderInitializer psInit = {};
	{

		{
			vsLoader.FileLoad(_ShaderName.VS.File.c_str());

			vsInit.Data = vsLoader.GetData();
			vsInit.Size = vsLoader.GetSize();
			vsInit.Name = _ShaderName.VS.GetShaderName();
			vsInit.Device = _Device->GetDevice();
			vsInit.Type = EShaderType::SHADER_TYPE_VERTEX;
		}

		{
			hsLoader.FileLoad(_ShaderName.HS.File.c_str());

			hsInit.Data = hsLoader.GetData();
			hsInit.Size = hsLoader.GetSize();
			hsInit.Name = _ShaderName.HS.GetShaderName();
			hsInit.Type = EShaderType::SHADER_TYPE_HULL;
		}

		{
			dsLoader.FileLoad(_ShaderName.DS.File.c_str());

			dsInit.Data = dsLoader.GetData();
			dsInit.Size = dsLoader.GetSize();
			dsInit.Name = _ShaderName.DS.GetShaderName();
			dsInit.Type = EShaderType::SHADER_TYPE_DOMAIN;
		}

		{
			gsLoader.FileLoad(_ShaderName.GS.File.c_str());

			gsInit.Data = gsLoader.GetData();
			gsInit.Size = gsLoader.GetSize();
			gsInit.Name = _ShaderName.GS.GetShaderName();
			gsInit.Type = EShaderType::SHADER_TYPE_GEOMETRY;
		}

		{
			psLoader.FileLoad(_ShaderName.PS.File.c_str());

			psInit.Data = psLoader.GetData();
			psInit.Size = psLoader.GetSize();
			psInit.Name = _ShaderName.PS.GetShaderName();
			psInit.Type = EShaderType::SHADER_TYPE_PIXEL;
		}
	}

	initializer.Device = _Device;
	initializer.Flag = Glue::ERootSignatureFlag::ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	initializer.pParameters = &RootParam;
	initializer.NumParameters = 1;
	initializer.pStaticSamplers = &Sampler;
	initializer.NumStaticSamplers = 1;

	initializer.pPipelineDesc = &Desc;

	initializer.VertexShaderInit = &vsInit;
	initializer.HullShaderInit = &hsInit;
	initializer.DomainShaderInit = &dsInit;
	initializer.GeometryShaderInit = &gsInit;
	initializer.PixelShaderInit = &psInit;

	{
		using String = std::string;
		String name = {};
		if (vsInit.Name != INVALID_HASH) name += RHash160(vsInit.Name);
		if (hsInit.Name != INVALID_HASH) name += RHash160(hsInit.Name);
		if (dsInit.Name != INVALID_HASH) name += RHash160(dsInit.Name);
		if (gsInit.Name != INVALID_HASH) name += RHash160(gsInit.Name);
		if (psInit.Name != INVALID_HASH) name += RHash160(psInit.Name);

		initializer.Name = CHash160(name);
	}

	result = CreateRenderPipelineStateObject(&initializer);

	if (result)
	{
		PsoName::QuadPolygonRenderingPso = initializer.Name;
	}

	vsLoader.ResetData();
	hsLoader.ResetData();
	dsLoader.ResetData();
	gsLoader.ResetData();
	psLoader.ResetData();
	
	return result;
}

bool CResourceSubsystemImpl::CreateBrightnessPipelineStateObject(ATLANTIS_NAMESPACE::CDX12MainDevice* _Device, const ATLANTIS_NAMESPACE::FShaderNameBlock& _ShaderName)
{
	FPipelineStateInitializerParameters InitParam = {};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC Desc = {};
	{
		Desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		Desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		Desc.NumRenderTargets = 1;
		Desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		Desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

		Desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		Desc.SampleDesc.Count = 1;
		Desc.SampleDesc.Quality = 0;

		Desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	}

	D3D12_DESCRIPTOR_RANGE Range = {};
	{
		Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		Range.BaseShaderRegister = 0;
		Range.NumDescriptors = 1;
	}

	D3D12_ROOT_PARAMETER RootParam = {};
	{
		RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		RootParam.DescriptorTable.NumDescriptorRanges = 1;
		RootParam.DescriptorTable.pDescriptorRanges = &Range;
		RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	}

	D3D12_STATIC_SAMPLER_DESC Sampler = {};
	{
		Sampler = CD3DX12_STATIC_SAMPLER_DESC(0);
	}

	InitParam.pPipelineStateDesc = &Desc;
	InitParam.pRootParam = &RootParam;
	InitParam.RootParamNum = 1;
	InitParam.pStaticSamplerDesc = &Sampler;
	InitParam.SamplerParamNum = 1;

	return CreatePipelineStateObjectImpl(PsoName::BrightnessThresholdPso, _Device, _ShaderName, &InitParam);
}

bool CResourceSubsystemImpl::CreateBlurPipelineStateObject(ATLANTIS_NAMESPACE::CDX12MainDevice* _Device, const ATLANTIS_NAMESPACE::FShaderNameBlock& _ShaderName)
{
	FPipelineStateInitializerParameters InitParam = {};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC Desc = {};
	{
		Desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		Desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		Desc.NumRenderTargets = 1;
		Desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		Desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

		Desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		Desc.SampleDesc.Count = 1;
		Desc.SampleDesc.Quality = 0;

		Desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	}

	D3D12_DESCRIPTOR_RANGE Range[2] = {};
	{
		Range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		Range[0].BaseShaderRegister = 0;
		Range[0].NumDescriptors = 1;
		Range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		Range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		Range[1].BaseShaderRegister = 0;
		Range[1].NumDescriptors = 2;
		Range[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}

	D3D12_ROOT_PARAMETER RootParam = {};
	{
		RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		RootParam.DescriptorTable.NumDescriptorRanges = 2;
		RootParam.DescriptorTable.pDescriptorRanges = Range;
		RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	}

	D3D12_STATIC_SAMPLER_DESC Sampler = {};
	{
		Sampler = CD3DX12_STATIC_SAMPLER_DESC(0);
	}

	InitParam.pPipelineStateDesc = &Desc;
	InitParam.pRootParam = &RootParam;
	InitParam.RootParamNum = 1;
	InitParam.pStaticSamplerDesc = &Sampler;
	InitParam.SamplerParamNum = 1;

	return CreatePipelineStateObjectImpl(PsoName::BlurRenderingPso,_Device,_ShaderName,&InitParam);
}

bool CResourceSubsystemImpl::CreateFetchColorPipelineStateObject(ATLANTIS_NAMESPACE::CDX12MainDevice* _Device, const ATLANTIS_NAMESPACE::FShaderNameBlock& _ShaderName)
{
	FPipelineStateInitializerParameters InitParam = {};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC Desc = {};
	{
		Desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		Desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		Desc.NumRenderTargets = 1;
		Desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		Desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

		Desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		Desc.SampleDesc.Count = 1;
		Desc.SampleDesc.Quality = 0;

		Desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	}

	D3D12_DESCRIPTOR_RANGE Range = {};
	{
		Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		Range.BaseShaderRegister = 0;
		Range.NumDescriptors = 5;
		Range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}

	D3D12_ROOT_PARAMETER RootParam = {};
	{
		RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		RootParam.DescriptorTable.NumDescriptorRanges = 1;
		RootParam.DescriptorTable.pDescriptorRanges = &Range;
		RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	}

	D3D12_STATIC_SAMPLER_DESC Sampler = {};
	{
		Sampler = CD3DX12_STATIC_SAMPLER_DESC(0);
	}

	InitParam.pPipelineStateDesc = &Desc;
	InitParam.pRootParam = &RootParam;
	InitParam.RootParamNum = 1;
	InitParam.pStaticSamplerDesc = &Sampler;
	InitParam.SamplerParamNum = 1;

	return CreatePipelineStateObjectImpl(PsoName::FetchColorPso, _Device, _ShaderName, &InitParam);
}

bool CResourceSubsystemImpl::CreateRenderPipelineStateObject(const BIFROST_NAMESPACE::FPipelineStateObjectInitializer* _Init)
{
	ManagerPtr manager = GetPipelineStateObjectManager();
	if (manager == nullptr) { return false; }

	FResourceHandle handle = {};

	handle = manager->SearchCreateResource(_Init);
	if(manager->IsInvalidHandle(handle))
	{
		return false;
	}

	return true;
}

bool CResourceSubsystemImpl::CreatePipelineStateObjectImpl(Hash160& _OutPsoName,ATLANTIS_NAMESPACE::CDX12MainDevice* _Device, const ATLANTIS_NAMESPACE::FShaderNameBlock& _ShaderName, const FPipelineStateInitializerParameters* _InitParam)
{
	FPipelineStateObjectInitializer initializer = {};

	bool result = false;

	CFileLoader vsLoader = {};
	CFileLoader hsLoader = {};
	CFileLoader dsLoader = {};
	CFileLoader gsLoader = {};
	CFileLoader psLoader = {};

	FVertexShaderInitializer vsInit = {};
	FHullShaderInitializer hsInit = {};
	FDomainShaderInitializer dsInit = {};
	FGeometryShaderInitializer gsInit = {};
	FPixelShaderInitializer psInit = {};
	{

		{
			result = vsLoader.FileLoad(_ShaderName.VS.File.c_str());
			if (!result)
			{
				PRINT("Vertex Shader Load Failed.\n");
			}

			vsInit.Data = vsLoader.GetData();
			vsInit.Size = vsLoader.GetSize();
			vsInit.Name = _ShaderName.VS.GetShaderName();
			vsInit.Device = _Device->GetDevice();
			vsInit.Type = EShaderType::SHADER_TYPE_VERTEX;
		}

		{
			hsLoader.FileLoad(_ShaderName.HS.File.c_str());

			hsInit.Data = hsLoader.GetData();
			hsInit.Size = hsLoader.GetSize();
			hsInit.Name = _ShaderName.HS.GetShaderName();
			hsInit.Type = EShaderType::SHADER_TYPE_HULL;
		}

		{
			dsLoader.FileLoad(_ShaderName.DS.File.c_str());

			dsInit.Data = dsLoader.GetData();
			dsInit.Size = dsLoader.GetSize();
			dsInit.Name = _ShaderName.DS.GetShaderName();
			dsInit.Type = EShaderType::SHADER_TYPE_DOMAIN;
		}

		{
			gsLoader.FileLoad(_ShaderName.GS.File.c_str());

			gsInit.Data = gsLoader.GetData();
			gsInit.Size = gsLoader.GetSize();
			gsInit.Name = _ShaderName.GS.GetShaderName();
			gsInit.Type = EShaderType::SHADER_TYPE_GEOMETRY;
		}

		{
			result = psLoader.FileLoad(_ShaderName.PS.File.c_str());
			if (!result)
			{
				PRINT("Pixel Shader Load Failed.\n");
			}

			psInit.Data = psLoader.GetData();
			psInit.Size = psLoader.GetSize();
			psInit.Name = _ShaderName.PS.GetShaderName();
			psInit.Type = EShaderType::SHADER_TYPE_PIXEL;
		}
	}

	initializer.Device = _Device;
	initializer.Flag = Glue::ERootSignatureFlag::ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	initializer.pParameters = _InitParam->pRootParam;
	initializer.NumParameters = _InitParam->RootParamNum;
	initializer.pStaticSamplers = _InitParam->pStaticSamplerDesc;
	initializer.NumStaticSamplers = _InitParam->SamplerParamNum;

	initializer.pPipelineDesc = _InitParam->pPipelineStateDesc;

	initializer.VertexShaderInit = &vsInit;
	initializer.HullShaderInit = &hsInit;
	initializer.DomainShaderInit = &dsInit;
	initializer.GeometryShaderInit = &gsInit;
	initializer.PixelShaderInit = &psInit;

	{
		using String = std::string;
		String name = {};
		if (vsInit.Name != INVALID_HASH) name += RHash160(vsInit.Name);
		if (hsInit.Name != INVALID_HASH) name += RHash160(hsInit.Name);
		if (dsInit.Name != INVALID_HASH) name += RHash160(dsInit.Name);
		if (gsInit.Name != INVALID_HASH) name += RHash160(gsInit.Name);
		if (psInit.Name != INVALID_HASH) name += RHash160(psInit.Name);

		initializer.Name = CHash160(name);
	}

	result = CreateRenderPipelineStateObject(&initializer);

	if (result)
	{
		_OutPsoName = initializer.Name;
	}

	vsLoader.ResetData();
	hsLoader.ResetData();
	dsLoader.ResetData();
	gsLoader.ResetData();
	psLoader.ResetData();

	return result;
}
