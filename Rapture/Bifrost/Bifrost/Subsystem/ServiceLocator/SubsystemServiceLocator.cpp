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

const IResourceSubsystem* CSubsystemServiceLocator::GetResourceSubsystem()
{
	return s_Dominator->GetResourceSubsystem();
}

IResourceSubsystem* CSubsystemServiceLocator::GetResourceSubsystemEdit()
{
	return s_Dominator->GetResourceSubsystemEdit();
}

const CUpdateProcessorSubsystem* CSubsystemServiceLocator::GetUpdaterSubsystem()
{
	return s_Dominator->GetUpdaterSubsystem();
}

CUpdateProcessorSubsystem* CSubsystemServiceLocator::GetUpdaterSubsystemEdit()
{
	return s_Dominator->GetUpdaterSubsystemEdit();
}

const CRenderingSubsystem* CSubsystemServiceLocator::GetRenderingSubsystem()
{
	return s_Dominator->GetRenderingSubsystem();
}

CRenderingSubsystem* CSubsystemServiceLocator::GetRenderingSubsystemEdit()
{
	return s_Dominator->GetRenderingSubsystemEdit();
}

const CActorSubsystem* CSubsystemServiceLocator::GetActorSubsystem()
{
	return s_Dominator->GetActorSubsystem();;
}

CActorSubsystem* CSubsystemServiceLocator::GetActorSubsystemEdit()
{
	return s_Dominator->GetActorSubsystemEdit();
}

const CCameraSubsystem* CSubsystemServiceLocator::GetCameraSubsystem()
{
	return s_Dominator->GetCameraSubsystem();
}

CCameraSubsystem* CSubsystemServiceLocator::GetCameraSubsystemEdit()
{
	return s_Dominator->GetCameraSubsystemEdit();;
}

const CLightSubsystem* CSubsystemServiceLocator::GetLightSubsystem()
{
	return s_Dominator->GetLightSubsystem();
}

CLightSubsystem* CSubsystemServiceLocator::GetLightSubsystemEdit()
{
	return s_Dominator->GetLightSubsystemEdit();
}
