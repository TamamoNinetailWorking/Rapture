#pragma once

#include <vector>

EDENS_NAMESPACE_BEGIN

class CResourceManager;

EDENS_NAMESPACE_END

//ATLANTIS_NAMESPACE_BEGIN
BIFROST_NAMESPACE_BEGIN

class CResourceManagementSubsystem
{
public:

	struct FInitializer
	{
		uint32 ResourceTypeNum = 0;
	};

	bool Initialize(const FInitializer& _Initializer);
	void Finalize();

	template<class TManager>
	bool SetupManager(uint32 _Type);

	CResourceManagementSubsystem() {};
	~CResourceManagementSubsystem() {};

private:

	friend class IResourceSubsystem;

	//bool CreateManagers(const FInitializer& _Initializer);

	typedef std::vector<EDENS_NAMESPACE::CResourceManager*> ResMagList;

	ResMagList m_Managers = {};

};

BIFROST_NAMESPACE_END
//ATLANTIS_NAMESPACE_END

#include "ResourceManagementSubsystem.inl"