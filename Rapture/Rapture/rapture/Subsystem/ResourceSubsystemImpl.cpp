#include "ResourceSubsystemImpl.h"
#include <Bifrost/Subsystem/Resource/ResourceManagementSubsystem.h>
#include <eden/include/Resource/ResourceManager.h>

#include <Atlantis/DirectX12/MainDevice/MainDevice.h>

#include <eden/include/Resource/ResourceManagerPreDefine.h>
#include <Bifrost/Resource/DefaultResourceDefine.h>
#include <Atlantis/DirectX12/DirectX12BaseDefine.h>
#include <Atlantis/DirectX12/Texture/TextureResourceInitializer.h>

USING_BIFROST;
USING_ATLANTIS;
EDENS_NAMESPACE_USING;

using namespace std;

#define SUBSYTEM_IS_NULL() \
	if(m_Subsystem == nullptr) { return nullptr; } \

IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetTextureResourceManager()
{
	return GetManagerFromIndex(RESOURCE_TYPE_TEXTURE);
}

IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetMeshResourceManager()
{
	return GetManagerFromIndex(RESOURCE_TYPE_MESH);
}

IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetShaderResourceManager()
{
	return GetManagerFromIndex(RESOURCE_TYPE_SHADER);
}

IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetMaterialResourceManager()
{
	return GetManagerFromIndex(RESOURCE_TYPE_MATERIAL);
}

void CResourceSubsystemImpl::Release()
{
	m_Subsystem = nullptr;
}

void CResourceSubsystemImpl::SetSubsystem(CResourceManagementSubsystem* _Subsystem)
{
	m_Subsystem = _Subsystem;
}

bool CResourceSubsystemImpl::CreateDefaultTextureResource(const CDX12MainDevice* _Device)
{
	ManagerPtr manager = GetTextureResourceManager();
	if (manager == nullptr) { return false; }

	FResourceHandle handle = {};

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
		manager->SearchCreateResource(&initializer);

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
		manager->SearchCreateResource(&initializer);

		if (manager->IsInvalidHandle(handle))
		{
			return false;
		}
	}

	return true;
}

void CResourceSubsystemImpl::DeleteDefaultTextureResource()
{
	ManagerPtr manager = GetTextureResourceManager();
	if (manager == nullptr) { return; }

	manager->DeleteResource(DefaultResource::GrayGradationTextureResource);
	manager->DeleteResource(DefaultResource::BlackTextureResource);
	manager->DeleteResource(DefaultResource::WhiteTextureResource);
}

IResourceSubsystem::ManagerPtr CResourceSubsystemImpl::GetManagerFromIndex(EResourceManagementType _Type)
{
	SUBSYTEM_IS_NULL();
	uint32 index = _Type;
	return GetManager(index);
}
