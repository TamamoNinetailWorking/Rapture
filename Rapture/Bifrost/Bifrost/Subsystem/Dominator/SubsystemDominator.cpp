#include "SubsystemDominator.h"
#include <Bifrost/Subsystem/Resource/ResourceSubsystemInterface.h>

USING_BIFROST;

bool CSubsystemDominator::Initialize()
{
	return true;
}

void CSubsystemDominator::Finalize()
{

}

void CSubsystemDominator::SetResourceSubsystem(IResourceSubsystem* _ResSubsystem)
{
	m_ResourceSubsystem = _ResSubsystem;
}