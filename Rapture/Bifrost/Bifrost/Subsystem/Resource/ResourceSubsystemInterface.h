#pragma once

EDENS_NAMESPACE_BEGIN

class CResourceManager;

EDENS_NAMESPACE_END

//ATLANTIS_NAMESPACE_BEGIN
BIFROST_NAMESPACE_BEGIN

class CResourceManagementSubsystem;


class IResourceSubsystem
{
public:

	typedef EDENS_NAMESPACE::CResourceManager* ManagerPtr;

	virtual const ManagerPtr GetTextureResourceManager() const = 0;
	virtual ManagerPtr GetTextureResourceManagerEdit() = 0;

	virtual const ManagerPtr GetMeshResourceManager() const = 0;
	virtual ManagerPtr GetMeshResourceManagerEdit() = 0;

	virtual const ManagerPtr GetShaderResourceManager() const = 0;
	virtual ManagerPtr GetShaderResourceManagerEdit() = 0;
	
	virtual const ManagerPtr GetMaterialResourceManager() const = 0;
	virtual ManagerPtr GetMaterialResourceManagerEdit() = 0;

	virtual const ManagerPtr GetPipelineStateObjectManager() const = 0;
	virtual ManagerPtr GetPipelineStateObjectManagerEdit() = 0;

	virtual void Release() = 0;

protected:

	friend class CSubsystemDominator;

	const ManagerPtr GetManager(uint32 _Index) const;

	ManagerPtr GetManagerEdit(uint32 _Index);

	ObjectPtr(CResourceManagementSubsystem) m_Subsystem = nullptr;

};

BIFROST_NAMESPACE_END
//ATLANTIS_NAMESPACE_END