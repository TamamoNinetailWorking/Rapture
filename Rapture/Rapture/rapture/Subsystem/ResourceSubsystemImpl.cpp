#include "ResourceSubsystemImpl.h"
#include <Bifrost/Subsystem/Resource/ResourceManagementSubsystem.h>
#include <eden/include/Resource/ResourceManager.h>

#include <Atlantis/DirectX12/MainDevice/MainDevice.h>

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

const IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetTextureResourceManager() const
{
	return GetManagerFromIndex(RESOURCE_TYPE_TEXTURE);
}

IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetTextureResourceManagerEdit()
{
	return GetManagerFromIndexEdit(RESOURCE_TYPE_TEXTURE);
}

const IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetMeshResourceManager() const
{
	return GetManagerFromIndex(RESOURCE_TYPE_MESH);
}

IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetMeshResourceManagerEdit()
{
	return GetManagerFromIndexEdit(RESOURCE_TYPE_MESH);
}

const IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetShaderResourceManager() const
{
	return GetManagerFromIndex(RESOURCE_TYPE_SHADER);
}

IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetShaderResourceManagerEdit()
{
	return GetManagerFromIndexEdit(RESOURCE_TYPE_SHADER);
}

const IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetMaterialResourceManager() const
{
	return GetManagerFromIndex(RESOURCE_TYPE_MATERIAL);
}

IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetMaterialResourceManagerEdit()
{
	return GetManagerFromIndexEdit(RESOURCE_TYPE_MATERIAL);
}

const IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetPipelineStateObjectManager() const
{
	return GetManagerFromIndex(RESOURCE_TYPE_PSO);
}

IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetPipelineStateObjectManagerEdit()
{
	return GetManagerFromIndexEdit(RESOURCE_TYPE_PSO);
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
	uint32 index = _Type;
	return GetManager(index);
}

const IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetManagerFromIndex(EResourceManagementType _Type) const
{
	SUBSYTEM_IS_NULL();
	uint32 index = _Type;
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
		initializer.Format = Glue::EDataFormat::FORMAT_R8G8B8A8_UNORM;
		initializer.Width = 4;
		initializer.Height = 4;
		initializer.DepthOrArraySize = 1;
		initializer.Dimension = Glue::EResourceDimension::RESOURCE_DIMENSION_TEXTURE2D;
		initializer.RowPitch = initializer.SlicePitch * initializer.Height;
		initializer.SlicePitch = initializer.Width * sizeof(uint8) * 4;
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
		initializer.Height = 256;
		initializer.RowPitch = initializer.Width * sizeof(uint32);

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
		// �f�B�X�N���v�^�\�e�[�u�� << �V�F�[�_�[�ň������W�X�^�̐ݒ�

		descTableRange[0].NumDescriptors = 1;
		descTableRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		descTableRange[0].BaseShaderRegister = 0;
		descTableRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		descTableRange[1].NumDescriptors = 1;
		descTableRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		descTableRange[1].BaseShaderRegister = 1;
		descTableRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		// �e�N�X�`���p�e�[�u��
		descTableRange[2].NumDescriptors = 4; //(���ʂ̃e�N�X�`����sph��spa��toon)
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
		desc.BlendState.IndependentBlendEnable = false;// ������false�Ȃ�RenderTarget�̐ݒ�͍ŏ���1������OK�ɂȂ� Independent => �ʂ�


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
		desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // ���������i�߂����j���̗p
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