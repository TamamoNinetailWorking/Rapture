#include "PmdMaterialData.h"
#include "PmdPreDefine.h"
#include "PmdMaterialDataInitializer.h"

#include <vector>

#include <Atlantis/DirectX12/DirectXHelper/d3dx12.h>

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>
#include <Atlantis/DirectX12/DirectXPaste.h>
#include <eden/include/utility/ender_utility.h>
#include <Atlantis/Utility/FileUtility/FileUtility.h>
#include <Atlantis/Utility/FileUtility/FileTypeDefine.h>
#include <eden/include/utility/StringUtility.h>

#include <Atlantis/DirectX12/ConstantBuffer/ConstantBufferPreDefine.h>

#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Resource/ResourceSubsystemInterface.h>
#include <Atlantis/Resource/TextureResourceManager.h>
#include <Bifrost/Resource/DefaultResourceDefine.h>
#include <Atlantis/DirectX12/Texture/TextureResource.h>

#include <Bifrost/Resource/Texture/FileLoadTexture.h>

USING_BIFROST;
USING_ATLANTIS;
EDENS_NAMESPACE_USING;

using String = std::string;

struct FMaterialConstantData
{
	Glue::Vector3 Diffuse = {};
	float Alpha = 0.f;
	Glue::Vector3 SpecularColor = {};
	float Specularity = 0.f;
	Glue::Vector3 AmbientColor = {};
	float AmbientAlpha = 0.f;
};

struct FMaterialResourceData
{
	FResourceHandle ColorTexture = {};
	FResourceHandle SphereTexture = {};
	FResourceHandle SphereAddTexture = {};
	FResourceHandle ToonResource = {};
};

struct FMaterialData
{
	uint32 DrawIndex = 0;
	FMaterialResourceData Data = {};
};

class CPmdMaterialData::Impl
{
public:

	bool CreateMaterials(const FPmdMaterialInitializer* _Initializer);

	void Release();

private:

	bool CreateConstantData(const FPmdMaterialInitializer* _Initializer);
	bool CreateMaterialData(const FPmdMaterialInitializer* _Initializer);
	bool CreateDescriptorHeap(const FPmdMaterialInitializer* _Initializer);
	bool CreateShaderResourceView(const FPmdMaterialInitializer* _Initializer);

	void ReleaseMaterialResource();

public:

	ObjectPtr(ID3D12DescriptorHeap) m_DescriptorHeap = nullptr;

	uint32 m_HeapStride = 0;

	std::vector<FMaterialData> m_MaterialData = {};

private:

	ObjectPtr(ID3D12Resource) m_ConstantBuffer = nullptr;

};

CPmdMaterialData::CPmdMaterialData()
{
	m_Impl = new Impl();
}

CPmdMaterialData::~CPmdMaterialData()
{
	Delete(m_Impl);
	CResource::~CResource();
}

bool CPmdMaterialData::Initialize(const EDENS_NAMESPACE::FResourceInitializerBase* _Initializer)
{
	do 
	{
		const FPmdMaterialInitializer* initializer = PCast<const FPmdMaterialInitializer*>(_Initializer);
		CHECK_RESULT_BREAK(_Initializer);

		CHECK_RESULT_BREAK(m_Impl->CreateMaterials(initializer));

		m_ResourceName = _Initializer->Name;

		return true;
	} while (0);

	Finalize();
	return false;
}

void CPmdMaterialData::Finalize()
{
	SafeReleaseD3DPtr(m_Impl->m_DescriptorHeap);
	m_Impl->Release();
}

ID3D12DescriptorHeap* CPmdMaterialData::GetDescriptorHeap() const
{
	return m_Impl->m_DescriptorHeap;
}

ID3D12DescriptorHeap*const* CPmdMaterialData::GetDescriptorHeapPtr() const
{
	return &m_Impl->m_DescriptorHeap;
}

uint32 CPmdMaterialData::GetHeapStride() const
{
	return m_Impl->m_HeapStride;
}

uint32 CPmdMaterialData::GetDrawIndex(uint32 _Index) const
{
	return m_Impl->m_MaterialData.at(_Index).DrawIndex;
}

uint32 CPmdMaterialData::GetMaterialNum() const
{
	return SCast<uint32>(m_Impl->m_MaterialData.size());
}

bool CPmdMaterialData::Impl::CreateMaterials(const FPmdMaterialInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	m_MaterialData.resize(_Initializer->MaterialNum);
	
	CHECK_RESULT_FALSE(CreateConstantData(_Initializer));

	CHECK_RESULT_FALSE(CreateMaterialData(_Initializer));

	CHECK_RESULT_FALSE(CreateDescriptorHeap(_Initializer));

	CHECK_RESULT_FALSE(CreateShaderResourceView(_Initializer));

	return true;
}

void CPmdMaterialData::Impl::Release()
{
	SafeReleaseD3DPtr(m_DescriptorHeap);
	SafeReleaseD3DPtr(m_ConstantBuffer);
	ReleaseMaterialResource();
}

bool CPmdMaterialData::Impl::CreateConstantData(const FPmdMaterialInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(_Initializer->Device);

	uint32 bufferSize = sizeof(FMaterialConstantData);
	bufferSize = AlignBufferSize(bufferSize);

	CD3DX12_HEAP_PROPERTIES heapProp = {};
	heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize * _Initializer->MaterialNum);//256�o�C�g�A���C���Ȃ̂ŁA�C���[�W�͂��₷�����A���C�����g�̃p�f�B���O�ł��񂲂����������]��

	D3D_ERROR_CHECK(_Initializer->Device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_ConstantBuffer)
	));

	return true;
}

bool CPmdMaterialData::Impl::CreateMaterialData(const FPmdMaterialInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	uint8* mapMaterial = nullptr;
	if (FAILED(m_ConstantBuffer->Map(0, nullptr, (void**)&mapMaterial)))
	{
		return false;
	}

	const String& modelName = RHash160(_Initializer->Name);
	const String& toonMapDirectory = RHash160(_Initializer->ToonMapDirectory);

	uint32 bufferSize = sizeof(FMaterialConstantData);
	bufferSize = AlignBufferSize(bufferSize);

	for (uint32 index = 0; index < _Initializer->MaterialNum; ++index)
	{
		FMaterialData& matData = m_MaterialData[index];
		const FPmdMaterialData& srcData = _Initializer->Materials[index];

		CResourceManager* manager = CSubsystemServiceLocator::GetResourceSubsystemEdit()->GetTextureResourceManagerEdit();

		// �C���f�b�N�X�f�[�^
		matData.DrawIndex = srcData.IndiciesNum;

		// �萔�f�[�^
		{
			FMaterialConstantData* data = RCast<FMaterialConstantData*>(mapMaterial);

			data->Diffuse = srcData.Diffuse;
			data->Alpha = srcData.Alpha;
			data->SpecularColor = srcData.SpecularColor;
			data->Specularity = srcData.Specularity;
			data->AmbientColor = srcData.AmbientColor;
			data->AmbientAlpha = 1.0f;

			mapMaterial += bufferSize;
		}

		// �e�N�X�`���f�[�^
		{
			const String& texPath = srcData.TexFilePath;

			String textureName = {};
			String sphName = {};
			String spaName = {};

			// �e�N�X�`�����𕪗�
			{
				constexpr char splitter = '*';
				if (StringUtility::ExitChara(texPath, splitter))
				{
					auto namepair = FileUtility::SeparateFileName(texPath, '*');
					auto extensionFirst = FileUtility::GetExtension(namepair.first);

					if (FileUtility::CompareExtension(extensionFirst, FileUtility::EFileExtensionType::FILE_TYPE_SPH))
					{
						textureName = namepair.second;
						sphName = namepair.first;
					}
					else if (FileUtility::CompareExtension(extensionFirst, FileUtility::EFileExtensionType::FILE_TYPE_SPA))
					{
						textureName = namepair.second;
						spaName = namepair.first;
					}
					else
					{
						textureName = namepair.first;

						auto extensionSecond = FileUtility::GetExtension(namepair.second);

						if (FileUtility::CompareExtension(extensionSecond, FileUtility::EFileExtensionType::FILE_TYPE_SPH))
						{
							sphName = namepair.second;
						}
						else if (FileUtility::CompareExtension(extensionSecond, FileUtility::EFileExtensionType::FILE_TYPE_SPA))
						{
							spaName = namepair.second;
						}
					}

				}
				else
				{
					auto extension = FileUtility::GetExtension(texPath);
					if (FileUtility::CompareExtension(extension, FileUtility::EFileExtensionType::FILE_TYPE_SPH))
					{
						sphName = texPath;
					}
					else if (FileUtility::CompareExtension(extension, FileUtility::EFileExtensionType::FILE_TYPE_SPA))
					{
						spaName = texPath;
					}
					else
					{
						textureName = texPath;
					}
				}

			}


			CFileLoadTexture loader = {};
			CFileLoadTexture::FInitializer initializer = {};
			initializer.Device = _Initializer->Device;


			// �J���[�e�N�X�`��
			{
				const String& texFolderFilePath = FileUtility::ConvertTexturePathWithModel(modelName, textureName);

				initializer.FileName = CHash160(texFolderFilePath);

				if (loader.Initialize(initializer))
				{
					matData.Data.ColorTexture = loader.GetResourceHandle();
				}
				else
				{
					matData.Data.ColorTexture = manager->SearchResourceHandle(DefaultResource::WhiteTextureResource);
				}

				loader.Finalize();
			}

			// �X�t�B�A�e�N�X�`��
			{
				const String& texFolderFilePath = FileUtility::ConvertTexturePathWithModel(modelName, sphName);

				initializer.FileName = CHash160(texFolderFilePath);

				if (loader.Initialize(initializer))
				{
					matData.Data.SphereTexture = loader.GetResourceHandle();
				}
				else
				{
					matData.Data.SphereTexture = manager->SearchResourceHandle(DefaultResource::WhiteTextureResource);
				}

				loader.Finalize();
			}

			// ���Z�X�t�B�A�e�N�X�`��
			{
				const String& texFolderFilePath = FileUtility::ConvertTexturePathWithModel(modelName, spaName);

				initializer.FileName = CHash160(texFolderFilePath);

				if (loader.Initialize(initializer))
				{
					matData.Data.SphereAddTexture = loader.GetResourceHandle();
				}
				else
				{
					matData.Data.SphereAddTexture = manager->SearchResourceHandle(DefaultResource::BlackTextureResource);
				}

				loader.Finalize();
			}

			// �g�D�[���e�N�X�`��
			{
				constexpr uint8 toonTexNameLength = 16;
				char toonTexName[toonTexNameLength] = {};

				sprintf_s(toonTexName, "toon%02d.bmp", SCast<uint8>(srcData.ToonIndex + 1));

				String toonFolderFilePath(toonMapDirectory);
				toonFolderFilePath += toonTexName;

				initializer.FileName = CHash160(toonFolderFilePath);

				if (loader.Initialize(initializer))
				{
					matData.Data.ToonResource = loader.GetResourceHandle();
				}
				else
				{
					matData.Data.ToonResource = manager->SearchResourceHandle(DefaultResource::GrayGradationTextureResource);
				}

				loader.Finalize();
			}

		}

	}

	m_ConstantBuffer->Unmap(0, nullptr);

	return true;
}

bool CPmdMaterialData::Impl::CreateDescriptorHeap(const FPmdMaterialInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(_Initializer->Device);

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;
	desc.NumDescriptors = SCast<uint32>(m_MaterialData.size() * 5);//1 + 4 = �萔�o�b�t�@�ƃe�N�X�`���̖���
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	D3D_ERROR_CHECK(_Initializer->Device->CreateDescriptorHeap(
		&desc,
		IID_PPV_ARGS(&m_DescriptorHeap)
	));
	return true;
}

bool CPmdMaterialData::Impl::CreateShaderResourceView(const FPmdMaterialInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	
	ID3D12Device* device = _Initializer->Device;
	CHECK_RESULT_FALSE(device);

	// �r���[�̍쐬
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	uint32 bufferSize = AlignBufferSize(sizeof(FMaterialConstantData));
	cbvDesc.SizeInBytes = bufferSize;
	cbvDesc.BufferLocation = m_ConstantBuffer->GetGPUVirtualAddress();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	
	D3D12_CPU_DESCRIPTOR_HANDLE descHandle = m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	uint32 handleIncrement = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	CResourceManager* manager = CSubsystemServiceLocator::GetResourceSubsystem()->GetTextureResourceManager();


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
	};

	for (uint32 index = 0; index < m_MaterialData.size(); ++index)
	{

		const auto& data = m_MaterialData.at(index).Data;

		// �R���X�^���g�o�b�t�@
		{
			device->CreateConstantBufferView(
				&cbvDesc,
				descHandle
			);
			
			descHandle.ptr += handleIncrement;
		}

		// �J���[�e�N�X�`��
		{
			CreateSRV(data.ColorTexture);
		}

		// �X�t�B�A�e�N�X�`��
		{
			CreateSRV(data.SphereTexture);
		}

		// ���Z�X�t�B�A�e�N�X�`��
		{
			CreateSRV(data.SphereAddTexture);
		}

		// �g�D�[���e�N�X�`��
		{
			CreateSRV(data.ToonResource);
		}

		cbvDesc.BufferLocation += bufferSize;
	}

	m_HeapStride = handleIncrement * 5; // �萔 + �e�N�X�`��4��

	return true;
}

void CPmdMaterialData::Impl::ReleaseMaterialResource()
{
	CResourceManager* manager = CSubsystemServiceLocator::GetResourceSubsystem()->GetTextureResourceManager();
	for (auto& element : m_MaterialData)
	{
		auto& data = element.Data;
		manager->DeleteResource(data.ColorTexture);
		manager->DeleteResource(data.SphereTexture);
		manager->DeleteResource(data.SphereAddTexture);
		manager->DeleteResource(data.ToonResource);
	}

	m_MaterialData.resize(0);
}
