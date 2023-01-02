#pragma once

#include <BIFROST/Subsystem/Resource/ResourceSubsystemInterface.h>
#include <rapture/Environment/ResourceTypeDefine.h>

ATLANTIS_NAMESPACE_BEGIN

class CDX12MainDevice;

ATLANTIS_NAMESPACE_END

class CResourceSubsystemImpl : public BIFROST_NAMESPACE::IResourceSubsystem
{
public:

	const ManagerPtr GetTextureResourceManager() const override;
	ManagerPtr GetTextureResourceManagerEdit() override;
	
	const ManagerPtr GetMeshResourceManager() const override;
	ManagerPtr GetMeshResourceManagerEdit() override;
	
	const ManagerPtr GetShaderResourceManager() const override;
	ManagerPtr GetShaderResourceManagerEdit() override;
	
	const ManagerPtr GetMaterialResourceManager() const override;
	ManagerPtr GetMaterialResourceManagerEdit() override;

	void Release() override;

	void SetSubsystem(BIFROST_NAMESPACE::CResourceManagementSubsystem* _Subsystem);

	bool CreateDefaultTextureResource(const ATLANTIS_NAMESPACE::CDX12MainDevice* _Device);

private:

	void DeleteDefaultTextureResource();

	const ManagerPtr GetManagerFromIndex(EResourceManagementType _Type) const;
	ManagerPtr GetManagerFromIndexEdit(EResourceManagementType _Type);

};