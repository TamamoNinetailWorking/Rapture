#include "ResourceManagementSubsystem.h"

#include <eden/include/Resource/ResourceManager.h>
#include <Bifrost/Subsystem/Resource/ResourceManagementSubsystemInitializer.h>
#include <Bifrost/Resource/DefaultResourceDefine.h>

USING_BIFROST;
EDENS_NAMESPACE_USING;

bool CResourceManagementSubsystem::Initialize(const FSubsystemInitializerBase* _Initializer)
{
	do
	{
		const FResourceManagementSubsystemInitializer* initializer = PCast<const FResourceManagementSubsystemInitializer*>(_Initializer);
		CHECK_RESULT_FALSE(initializer);

		if (initializer->ResourceTypeNum <= 0)
		{
			break;
		}

		m_Managers.resize(initializer->ResourceTypeNum);

		//CHECK_RESULT_BREAK(CreateManagers(_Initializer));

		return true;
	} while (0);

	Finalize();
	return false;
}

void CResourceManagementSubsystem::Finalize()
{
	for (auto& elem : m_Managers)
	{
		if (elem == nullptr) { continue; }

		elem->Finalize();
		delete elem;
	}
	m_Managers.resize(0);
}



//bool CResourceManagementSubsystem::CreateManagers(const FInitializer& _Initializer)
//{
//	m_Managers.resize(_Initializer.ResourceTypeNum);
//
//	for (auto& elem : m_Managers)
//	{
//		elem = new TManager();
//		CHECK_RESULT_FALSE(elem);
//
//		CHECK_RESULT_FALSE(elem->Initialize());
//	}
//
//	return true;
//}
