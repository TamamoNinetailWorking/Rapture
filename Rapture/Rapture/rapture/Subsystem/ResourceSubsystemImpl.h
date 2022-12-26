#pragma once

#include <BIFROST/Subsystem/Resource/ResourceSubsystemInterface.h>
#include <rapture/Environment/ResourceTypeDefine.h>

ATLANTIS_NAMESPACE_BEGIN

class CDX12MainDevice;

ATLANTIS_NAMESPACE_END

class CResourceSubsystemImpl : public BIFROST_NAMESPACE::IResourceSubsystem
{
public:

	ManagerPtr GetTextureResourceManager() override;
	ManagerPtr GetMeshResourceManager() override;
	ManagerPtr GetShaderResourceManager() override;
	ManagerPtr GetMaterialResourceManager() override;

	void Release() override;

	void SetSubsystem(BIFROST_NAMESPACE::CResourceManagementSubsystem* _Subsystem);

	bool CreateDefaultTextureResource(const ATLANTIS_NAMESPACE::CDX12MainDevice* _Device);

private:

	void DeleteDefaultTextureResource();

	ManagerPtr GetManagerFromIndex(EResourceManagementType _Type);
};