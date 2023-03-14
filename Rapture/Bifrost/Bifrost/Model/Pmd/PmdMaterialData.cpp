#include "PmdMaterialData.h"
#include "PmdPreDefine.h"
#include "PmdMaterialDataInitializer.h"
#include "PmdMaterialDefine.h"

#include <vector>

#include <Atlantis/DirectX12/DirectXHelper/d3dx12.h>
#include <Atlantis/DirectX12/MainDevice/MainDevice.h>

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>
#include <eden/include/utility/ender_utility.h>

#include <Atlantis/DirectX12/ConstantBuffer/ConstantBufferPreDefine.h>

#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Resource/ResourceSubsystemInterface.h>
#include <Atlantis/Resource/TextureResourceManager.h>
#include <Bifrost/Resource/DefaultResourceDefine.h>
#include <Atlantis/DirectX12/Texture/TextureResource.h>


#include <Bifrost/Model/Pmd/PmdMaterialResource.h>
#include <Bifrost/Model/Pmd/PmdMaterialResourceInitializer.h>
#include <Bifrost/Resource/Manager/MaterialManager.h>

#include <Bifrost/Resource/PSO/PipelineStateObject.h>
#include <Bifrost/Resource/PSO/PipelineStateObjectInitializer.h>
#include <Bifrost/Resource/Manager/PipelineStateObjectManager.h>


USING_BIFROST;
USING_ATLANTIS;
EDENS_NAMESPACE_USING;

using String = std::string;

#define DESC_COMBINE

struct FMaterialData
{
	uint32 DrawIndex = 0;
	FResourceHandle m_MaterialHandle = {};
};

class CPmdMaterialData::Impl
{
public:

	bool CreateMaterials(ID3D12DescriptorHeap*& _MatHeap, ID3D12DescriptorHeap*& _BufHeap, uint32& _Stride, const FPmdMaterialInitializer* _Initializer);

	bool CreatePipelineStateObject(const FPmdMaterialInitializer* _Initializer);

	void Release();

	const CPipelineStateObject* GetStateObject() const;

private:

	bool CreateMaterials(const FPmdMaterialInitializer* _Initializer);

	bool CreateDescriptorHeap(ID3D12DescriptorHeap*& _MatHeap, ID3D12DescriptorHeap*& _BufHeap, const FPmdMaterialInitializer* _Initializer);
	bool CreateShaderResourceView(ID3D12DescriptorHeap*& _MatHeap, ID3D12DescriptorHeap*& _BufHeap, uint32& _Stride, const FPmdMaterialInitializer* _Initializer);

	void ReleaseMaterialResource();

	void ReleasePipelineStateObject();

	bool CreatePipelineStateObjectImpl(const FPmdMaterialInitializer* _Initializer);

public:

	ObjectPtr(ID3D12Resource) m_VertexConstantBuffer = nullptr;
	ObjectPtr(FSceneData) m_SceneData = nullptr;

	std::vector<FMaterialData> m_MaterialData = {};

	FResourceHandle m_PsoHandle = {};

private:

};

CPmdMaterialData::CPmdMaterialData()
{
	m_Impl = new Impl();
}

CPmdMaterialData::~CPmdMaterialData()
{
	Delete(m_Impl);
	//CResource::~CResource();
}

bool CPmdMaterialData::Initialize(const ATLANTIS_NAMESPACE::FMaterialInterfaceInitializerBase* _Initializer)
{
	do 
	{
		const FPmdMaterialInitializer* initializer = PCast<const FPmdMaterialInitializer*>(_Initializer);
		CHECK_RESULT_BREAK(_Initializer);

		uint32 stride = 0;
		CHECK_RESULT_BREAK(m_Impl->CreateMaterials(m_MaterialDescriptorHeap,m_BufferDescriptorHeap, stride,initializer));
		SetHeapStride(stride);

		CHECK_RESULT_BREAK(m_Impl->CreatePipelineStateObject(initializer));

		return true;
	} while (0);

	Finalize();
	return false;
}

void CPmdMaterialData::Finalize()
{
	IMaterialInterface::Finalize();
	m_Impl->Release();
}

void CPmdMaterialData::SetGeometryBuffer(FMaterialGeometryBufferBase* _Buffer)
{
	m_Impl->m_SceneData = DCast<FSceneData*>(_Buffer);
}

uint32 CPmdMaterialData::GetDrawIndex(uint32 _Index) const
{
	return m_Impl->m_MaterialData.at(_Index).DrawIndex;
}

uint32 CPmdMaterialData::GetMaterialNum() const
{
	return SCast<uint32>(m_Impl->m_MaterialData.size());
}

CGraphicsPipeline* CPmdMaterialData::GetGraphicsPipeline() const
{
	return m_Impl->GetStateObject()->GetPipeline();
}

CRootSignature* CPmdMaterialData::GetRootSignature() const
{
	return m_Impl->GetStateObject()->GetRootSignature();
}

const ATLANTIS_NAMESPACE::FMaterialGeometryBufferBase* CPmdMaterialData::GetGeometryBuffer() const
{
	return m_Impl->m_SceneData;
}

ATLANTIS_NAMESPACE::FMaterialGeometryBufferBase* CPmdMaterialData::GetGeometryBufferEdit() const
{
	return m_Impl->m_SceneData;
}

bool CPmdMaterialData::Impl::CreateMaterials(ID3D12DescriptorHeap*& _MatHeap, ID3D12DescriptorHeap*& _BufHeap, uint32& _Stride,const FPmdMaterialInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	m_MaterialData.resize(_Initializer->MaterialNum);

	CHECK_RESULT_FALSE(CreateMaterials(_Initializer));

	CHECK_RESULT_FALSE(CreateDescriptorHeap(_MatHeap,_BufHeap, _Initializer));

	CHECK_RESULT_FALSE(CreateShaderResourceView(_MatHeap,_BufHeap,_Stride,_Initializer));

	return true;
}

bool CPmdMaterialData::Impl::CreatePipelineStateObject(const FPmdMaterialInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	CHECK_RESULT_FALSE(CreatePipelineStateObjectImpl(_Initializer));

	return true;
}

void CPmdMaterialData::Impl::Release()
{
	ReleasePipelineStateObject();
	ReleaseMaterialResource();
}

const CPipelineStateObject* CPmdMaterialData::Impl::GetStateObject() const
{
	const CResourceManager* manager = CSubsystemServiceLocator::GetResourceSubsystem()->GetPipelineStateObjectManager();
	return PCast<const CPipelineStateObject*>(manager->SearchResourceRef(m_PsoHandle));
}

bool CPmdMaterialData::Impl::CreateMaterials(const FPmdMaterialInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(_Initializer->Device);

	CResourceManager* manager = CSubsystemServiceLocator::GetResourceSubsystemEdit()->GetMaterialResourceManagerEdit();
	CHECK_RESULT_FALSE(manager);

	FPmdMaterialResourceInitializer initializer = {};
	initializer.Device = _Initializer->Device;
	initializer.ModelName = _Initializer->Name;
	initializer.ToonMapDirectory = _Initializer->ToonMapDirectory;
	initializer.Type = EMaterialType::MATERIAL_TYPE_PMD;

	constexpr uint32 size = SCast<uint32>(sizeof(FPmdMaterialData));
	CSHA1::DataBlob matData = {};
	matData.resize(size);

	for (uint32 index = 0; index < m_MaterialData.size(); ++index)
	{
		const FPmdMaterialData& srcData = _Initializer->Materials[index];
		FResourceHandle& data = m_MaterialData[index].m_MaterialHandle;
		
		initializer.Material = &srcData;

		//String matData = {};
		

		memcpy_s(&matData[0], size, &srcData, size);
		//matData.assign(RCast<const char*>(&srcData), sizeof(srcData));

		initializer.Name = CHash160(matData);
		//char name[256] = {};
		//sprintf_s(name, "%s%d", RHash160(initializer.ModelName), index);
		//initializer.Name = CHash160(name);

		data = manager->SearchCreateResource(&initializer);

		if (manager->IsInvalidHandle(data))
		{
			PRINT("CreateMaterials Invalid Handle.\n");
		}
	}


	return true;
}

bool CPmdMaterialData::Impl::CreateDescriptorHeap(ID3D12DescriptorHeap*& _MatHeap, ID3D12DescriptorHeap*& _BufHeap, const FPmdMaterialInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(_Initializer->Device);
	CHECK_RESULT_FALSE(_Initializer->Device->GetDevice());

	// �}�e���A���̃q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;
#ifndef DESC_COMBINE
	desc.NumDescriptors = SCast<uint32>(m_MaterialData.size() * 5);//(1 + 4) * MaterialNum = VertexShader�̒萔�o�b�t�@��PixelShader�̒萔�o�b�t�@�ƃe�N�X�`���̖���
#else
	desc.NumDescriptors = SCast<uint32>(m_MaterialData.size() * 5) + 1;
#endif
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	D3D_ERROR_CHECK(_Initializer->Device->GetDevice()->CreateDescriptorHeap(
		&desc,
		IID_PPV_ARGS(&_MatHeap)
	));

	//
	//------
	//�}�e���A���̃q�[�v��`��Ɏg�p�����ۂɃG���[���o�Ă���̂Ō���B
	//------
	//

	// �R���X�^���g�o�b�t�@�̃q�[�v
	desc.NumDescriptors = 1;

	D3D_ERROR_CHECK(_Initializer->Device->GetDevice()->CreateDescriptorHeap(
		&desc,
		IID_PPV_ARGS(&_BufHeap)
	));

	return true;
}

bool CPmdMaterialData::Impl::CreateShaderResourceView(ID3D12DescriptorHeap*& _MatHeap, ID3D12DescriptorHeap*& _BufHeap, uint32& _Stride, const FPmdMaterialInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	
	ID3D12Device* device = _Initializer->Device->GetDevice();
	CHECK_RESULT_FALSE(device);

	// �r���[�̍쐬
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	uint32 bufferSize = AlignBufferSize(sizeof(FMaterialConstantData));
	cbvDesc.SizeInBytes = bufferSize;
	//cbvDesc.BufferLocation = m_ConstantBuffer->GetGPUVirtualAddress();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	

	uint32 handleIncrement = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	
	// ���_�V�F�[�_�[�Ŏg�p����o�b�t�@
	{

#ifndef DESC_COMBINE
		D3D12_CPU_DESCRIPTOR_HANDLE bufHeap = _BufHeap->GetCPUDescriptorHandleForHeapStart();
#else
		D3D12_CPU_DESCRIPTOR_HANDLE bufHeap = _MatHeap->GetCPUDescriptorHandleForHeapStart();
#endif

		D3D12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(AlignBufferSize(sizeof(FSceneData::MainData)));

		D3D_ERROR_CHECK(device->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_VertexConstantBuffer)));

		m_SceneData = new FSceneData();
		CHECK_RESULT_FALSE(m_SceneData);

		m_SceneData->pData = new FSceneData::MainData();
		CHECK_RESULT_FALSE(m_SceneData);

		auto& data = *(m_SceneData->pData);

		data.World = DirectX::XMMatrixIdentity();
		data.View = DirectX::XMMatrixIdentity();
		data.ViewProjection = DirectX::XMMatrixIdentity();
		data.WorldViewProjection = DirectX::XMMatrixIdentity();

		auto temp = &m_SceneData->pData;
		D3D_ERROR_CHECK(m_VertexConstantBuffer->Map(0, nullptr, (void**)temp));

		D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
		desc.BufferLocation = m_VertexConstantBuffer->GetGPUVirtualAddress();
		desc.SizeInBytes = SCast<uint32>(m_VertexConstantBuffer->GetDesc().Width);

		device->CreateConstantBufferView(
			&desc,
			bufHeap);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE descHandle = _MatHeap->GetCPUDescriptorHandleForHeapStart();

#ifdef DESC_COMBINE
	descHandle.ptr += handleIncrement;
#endif

#undef DESC_COMBINE

	CResourceManager* manager = CSubsystemServiceLocator::GetResourceSubsystem()->GetTextureResourceManager();
	CHECK_RESULT_FALSE(manager);

	// �e�N�X�`���̃V�F�[�_�[���\�[�X�r���[���쐬���邽�߂̎菇
	auto CreateSRV = [&](const FResourceHandle& _Handle)
	{
		const CTextureResource* resource = PCast<const CTextureResource*>(manager->SearchResource(_Handle));

		if (resource != nullptr)
		{
			srvDesc.Format = resource->GetResource()->GetDesc().Format;
		}

		device->CreateShaderResourceView(
			resource->GetResource(),
			&srvDesc,
			descHandle
		);
		
		descHandle.ptr += handleIncrement;

		return;
	};

	CResourceManager* matManager = CSubsystemServiceLocator::GetResourceSubsystem()->GetMaterialResourceManager();
	CHECK_RESULT_FALSE(matManager);

	for (uint32 index = 0; index < m_MaterialData.size(); ++index)
	{
		const FPmdMaterialData& srcData = _Initializer->Materials[index];
		FMaterialData& data = m_MaterialData.at(index);

		const FResourceHandle& handle = data.m_MaterialHandle;

		const CPmdMaterialResource* material = PCast<const CPmdMaterialResource*>(matManager->SearchResource(handle));

		//const auto& data = m_MaterialData.at(index).Data;

		// �C���f�b�N�X������
		data.DrawIndex = srcData.IndiciesNum;

		// �R���X�^���g�o�b�t�@
		{
			cbvDesc.BufferLocation = material->GetConstantResource()->GetGPUVirtualAddress();

			device->CreateConstantBufferView(
				&cbvDesc,
				descHandle
			);
			
			descHandle.ptr += handleIncrement;
		}

		// �J���[�e�N�X�`��
		{
			//CreateSRV(data.ColorTexture);
			CreateSRV(material->GetDecalTextureHandle());
		}

		// �X�t�B�A�e�N�X�`��
		{
			//CreateSRV(data.SphereTexture);
			CreateSRV(material->GetSphereTextureHandle());
		}

		// ���Z�X�t�B�A�e�N�X�`��
		{
			//CreateSRV(data.SphereAddTexture);
			CreateSRV(material->GetSphereAddTextureHandle());
		}

		// �g�D�[���e�N�X�`��
		{
			//CreateSRV(data.ToonResource);
			CreateSRV(material->GetToonTextureHandle());
		}
	}

	_Stride = handleIncrement * 5; // �萔 + �e�N�X�`��4��

	return true;
}

void CPmdMaterialData::Impl::ReleaseMaterialResource()
{
	CResourceManager* manager = CSubsystemServiceLocator::GetResourceSubsystem()->GetMaterialResourceManager();
	for (auto& element : m_MaterialData)
	{
		auto& data = element.m_MaterialHandle;
		manager->DeleteResource(data);
		//manager->DeleteResource(data.ColorTexture);
		//manager->DeleteResource(data.SphereTexture);
		//manager->DeleteResource(data.SphereAddTexture);
		//manager->DeleteResource(data.ToonResource);
	}

	m_MaterialData.resize(0);
}

void CPmdMaterialData::Impl::ReleasePipelineStateObject()
{
	CResourceManager* manager = CSubsystemServiceLocator::GetResourceSubsystem()->GetPipelineStateObjectManager();

	manager->DeleteResource(m_PsoHandle);
}

bool CPmdMaterialData::Impl::CreatePipelineStateObjectImpl(const FPmdMaterialInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(_Initializer->PsoInit);

	CResourceManager* manager = CSubsystemServiceLocator::GetResourceSubsystem()->GetPipelineStateObjectManager();

	m_PsoHandle = manager->SearchCreateResource(_Initializer->PsoInit);
	CHECK_RESULT_FALSE(manager->IsValidHandle(m_PsoHandle));

	return true;
}
