#pragma once

#include <BIFROST/Subsystem/Resource/ResourceSubsystemInterface.h>
#include <rapture/Environment/ResourceTypeDefine.h>

#include <Atlantis/DirectX12/Shader/ShaderNameBlock.h>

ATLANTIS_NAMESPACE_BEGIN

class CDX12MainDevice;

ATLANTIS_NAMESPACE_END


BIFROST_NAMESPACE_BEGIN

struct FPipelineStateObjectInitializer;

BIFROST_NAMESPACE_END


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

	const ManagerPtr GetPipelineStateObjectManager() const override;
	ManagerPtr GetPipelineStateObjectManagerEdit() override;

	void Release() override;

	void SetSubsystem(BIFROST_NAMESPACE::CResourceManagementSubsystem* _Subsystem);

	bool CreateDefaultTextureResource(const ATLANTIS_NAMESPACE::CDX12MainDevice* _Device);

	bool CreatePmdPipelineStateObject(ATLANTIS_NAMESPACE::CDX12MainDevice* _Device, const ATLANTIS_NAMESPACE::FShaderNameBlock& _ShaderName);

private:

	bool CreateRenderPipelineStateObject(const BIFROST_NAMESPACE::FPipelineStateObjectInitializer* _Init);

	void DeleteDefaultTextureResource();

	void DeletePreDefPso();

	const ManagerPtr GetManagerFromIndex(EResourceManagementType _Type) const;
	ManagerPtr GetManagerFromIndexEdit(EResourceManagementType _Type);

};