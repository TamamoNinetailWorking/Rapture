#include "SubsystemServiceLocator.h"

#include <Bifrost/Subsystem/Dominator/SubsystemDominator.h>

USING_BIFROST;

ObjectPtr(CSubsystemDominator) CSubsystemServiceLocator::s_Dominator = nullptr;

void CSubsystemServiceLocator::SetSubsystemDominator(CSubsystemDominator* _Dominator)
{
	s_Dominator = _Dominator;
}

CSubsystemDominator* CSubsystemServiceLocator::GetSubsystemDominator()
{
	return s_Dominator;
}

IResourceSubsystem* CSubsystemServiceLocator::GetResourceSubsystem()
{
	return s_Dominator->GetResourceSubsystem();
}
