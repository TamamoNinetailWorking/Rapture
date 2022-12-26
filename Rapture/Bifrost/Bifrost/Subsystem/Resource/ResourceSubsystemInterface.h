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

	virtual ManagerPtr GetTextureResourceManager() = 0;
	virtual ManagerPtr GetMeshResourceManager() = 0;
	virtual ManagerPtr GetShaderResourceManager() = 0;
	virtual ManagerPtr GetMaterialResourceManager() = 0;

	virtual void Release() = 0;

protected:

	friend class CSubsystemDominator;

	ManagerPtr GetManager(uint32 _Index);

	ObjectPtr(CResourceManagementSubsystem) m_Subsystem = nullptr;

};

BIFROST_NAMESPACE_END
//ATLANTIS_NAMESPACE_END