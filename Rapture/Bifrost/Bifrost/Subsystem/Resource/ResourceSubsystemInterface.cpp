#include "ResourceSubsystemInterface.h"

#include "ResourceManagementSubsystem.h"

USING_BIFROST;

const IResourceSubsystem::ManagerPtr IResourceSubsystem::GetManager(uint32 _Index) const
{
	return m_Subsystem->m_Managers.at(_Index);
}

IResourceSubsystem::ManagerPtr IResourceSubsystem::GetManagerEdit(uint32 _Index)
{
	return m_Subsystem->m_Managers.at(_Index);
}
