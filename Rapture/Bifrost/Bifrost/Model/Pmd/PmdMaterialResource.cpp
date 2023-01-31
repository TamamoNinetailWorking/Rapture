#include "PmdMaterialResource.h"
#include "PmdMaterialDefine.h"
#include "PmdMaterialResourceInitializer.h"
#include "PmdPreDefine.h"
#include <Atlantis/DirectX12/ConstantBuffer/ConstantBufferPreDefine.h>

#include <d3d12.h>
#include <Atlantis/DirectX12/DirectX12BaseDefine.h>
#include <Atlantis/DirectX12/DirectXHelper/d3dx12.h>

#include <Atlantis/DirectX12/MainDevice/MainDevice.h>

#include <eden/include/utility/StringUtility.h>
#include <Atlantis/Utility/FileUtility/FileUtility.h>

#include <eden/include/Resource/ResourceManager.h>
#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Resource/ResourceSubsystemInterface.h>

#include <Bifrost/Resource/Texture/FileLoadTexture.h>
#include <Bifrost/Resource/DefaultResourceDefine.h>

USING_BIFROST;
USING_ATLANTIS;
EDENS_NAMESPACE_USING;

using String = std::string;

CPmdMaterialResource::CPmdMaterialResource() : CResource()
{

}

CPmdMaterialResource::~CPmdMaterialResource()
{
	Finalize();
	CResource::~CResource();
}

bool CPmdMaterialResource::Initialize(const EDENS_NAMESPACE::FResourceInitializerBase* _Initializer)
{
	do
	{
		const FPmdMaterialResourceInitializer* initializer = PCast<const FPmdMaterialResourceInitializer*>(_Initializer);
		CHECK_RESULT_BREAK(_Initializer);

		m_ResourceName = initializer->Name;

		CHECK_RESULT_BREAK(CreateConstantBuffer(initializer));

		CHECK_RESULT_BREAK(CreateMaterialTexture(initializer));

		return true;
	} while (0);

	Finalize();
	return false;
}

void CPmdMaterialResource::Finalize()
{
	CResourceManager* manager = CSubsystemServiceLocator::GetResourceSubsystemEdit()->GetTextureResourceManagerEdit();

	auto DeleteResource = [&](FResourceHandle& _Handle)
	{
		manager->DeleteResource(_Handle);
		_Handle = manager->GetInvalidHandle();
	};
	
	DeleteResource(m_ToonTexture);
	DeleteResource(m_SphereAddTexture);
	DeleteResource(m_SphereTexture);
	DeleteResource(m_DecalTexture);

	ATLANTIS_NAMESPACE::SafeReleaseD3DPtr(m_ConstantBuffer);
}

bool CPmdMaterialResource::CreateConstantBuffer(const FPmdMaterialResourceInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(_Initializer->Device);
	CHECK_RESULT_FALSE(_Initializer->Device->GetDevice());

	const FPmdMaterialData* srcData = _Initializer->Material;
	CHECK_RESULT_FALSE(srcData);

	uint32 bufferSize = sizeof(FMaterialConstantData);
	bufferSize = AlignBufferSize(bufferSize);

	CD3DX12_HEAP_PROPERTIES heapProp = {};
	heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);//256バイトアラインなので、イメージはしやすいがアライメントのパディングですんごいメモリが余る

	D3D_ERROR_CHECK(_Initializer->Device->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_ConstantBuffer)
	));

	FMaterialConstantData* mapMaterial = nullptr;
	D3D_ERROR_CHECK(m_ConstantBuffer->Map(0, nullptr, (void**)&mapMaterial));
	
	mapMaterial->Diffuse =			srcData->Diffuse;
	//mapMaterial->Diffuse =			Glue::Vector3(0.2f,8.f,1.0f);
	mapMaterial->Alpha =			srcData->Alpha;
	mapMaterial->SpecularColor =	srcData->SpecularColor;
	mapMaterial->Specularity =		srcData->Specularity;
	mapMaterial->AmbientColor =		srcData->AmbientColor;
	mapMaterial->AmbientAlpha =		1.0f;

	m_ConstantBuffer->Unmap(0, nullptr);

	return true;
}

bool CPmdMaterialResource::CreateMaterialTexture(const FPmdMaterialResourceInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(_Initializer->Device);

	const FPmdMaterialData* srcData = _Initializer->Material;
	CHECK_RESULT_FALSE(srcData);

	const String& modelName = RHash160(_Initializer->ModelName);
	const String& toonMapDirectory = RHash160(_Initializer->ToonMapDirectory);

	const String& texPath = srcData->TexFilePath;

	CResourceManager* manager = CSubsystemServiceLocator::GetResourceSubsystemEdit()->GetTextureResourceManagerEdit();

	String textureName = {};
	String sphName = {};
	String spaName = {};

	// テクスチャ名を分離
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
	initializer.Device = _Initializer->Device->GetDevice();

	// カラーテクスチャ
	{
		const String& texFolderFilePath = FileUtility::ConvertTexturePathWithModel(modelName, textureName);

		initializer.FileName = CHash160(texFolderFilePath);

		if (loader.Initialize(initializer))
		{
			m_DecalTexture = loader.GetResourceHandle();
		}
		else
		{
			m_DecalTexture = manager->SearchResourceHandle(DefaultResource::WhiteTextureResource);
		}

		loader.Finalize();
	}

	// スフィアテクスチャ
	{
		const String& texFolderFilePath = FileUtility::ConvertTexturePathWithModel(modelName, sphName);

		initializer.FileName = CHash160(texFolderFilePath);

		if (loader.Initialize(initializer))
		{
			m_SphereTexture = loader.GetResourceHandle();
		}
		else
		{
			m_SphereTexture = manager->SearchResourceHandle(DefaultResource::WhiteTextureResource);
		}

		loader.Finalize();
	}

	// 加算スフィアテクスチャ
	{
		const String& texFolderFilePath = FileUtility::ConvertTexturePathWithModel(modelName, spaName);

		initializer.FileName = CHash160(texFolderFilePath);

		if (loader.Initialize(initializer))
		{
			m_SphereAddTexture = loader.GetResourceHandle();
		}
		else
		{
			m_SphereAddTexture = manager->SearchResourceHandle(DefaultResource::BlackTextureResource);
		}

		loader.Finalize();
	}

	// トゥーンテクスチャ
	{
		constexpr uint8 toonTexNameLength = 16;
		char toonTexName[toonTexNameLength] = {};

		sprintf_s(toonTexName, "toon%02d.bmp", SCast<uint8>(srcData->ToonIndex + 1));

		String toonFolderFilePath(toonMapDirectory);
		toonFolderFilePath += toonTexName;

		initializer.FileName = CHash160(toonFolderFilePath);

		if (loader.Initialize(initializer))
		{
			m_ToonTexture = loader.GetResourceHandle();
		}
		else
		{
			m_ToonTexture = manager->SearchResourceHandle(DefaultResource::GrayGradationTextureResource);
		}

		loader.Finalize();
	}

	return true;
}

ID3D12Resource* CPmdMaterialResource::GetConstantResource() const
{
	return m_ConstantBuffer;
}

const EDENS_NAMESPACE::FResourceHandle& CPmdMaterialResource::GetDecalTextureHandle() const
{
	return m_DecalTexture;
}

const EDENS_NAMESPACE::FResourceHandle& CPmdMaterialResource::GetSphereTextureHandle() const
{
	return m_SphereTexture;
}

const EDENS_NAMESPACE::FResourceHandle& CPmdMaterialResource::GetSphereAddTextureHandle() const
{
	return m_SphereAddTexture;
}

const EDENS_NAMESPACE::FResourceHandle& CPmdMaterialResource::GetToonTextureHandle() const
{
	return m_ToonTexture;
}


