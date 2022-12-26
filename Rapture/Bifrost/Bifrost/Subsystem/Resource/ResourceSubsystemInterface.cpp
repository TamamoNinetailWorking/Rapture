#include "ResourceSubsystemInterface.h"

#include "ResourceManagementSubsystem.h"

USING_BIFROST;

IResourceSubsystem::ManagerPtr IResourceSubsystem::GetManager(uint32 _Index)
{
	return m_Subsystem->m_Managers.at(_Index);
}