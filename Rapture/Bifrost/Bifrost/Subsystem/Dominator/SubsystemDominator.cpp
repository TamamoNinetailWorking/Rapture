#include "SubsystemDominator.h"
//#include <Bifrost/Subsystem/Resource/ResourceSubsystemInterface.h>

USING_BIFROST;

bool CSubsystemDominator::Initialize()
{
	return true;
}

void CSubsystemDominator::Finalize()
{

}
#if 0
void CSubsystemDominator::SetResourceSubsystem(IResourceSubsystem* _ResSubsystem)
{
	m_ResourceSubsystem = _ResSubsystem;
}

void CSubsystemDominator::SetUpdaterSubsystem(CUpdateProcessorSubsystem* _UpdaterSubsystem)
{
	m_UpdaterSubsystem = _UpdaterSubsystem;
}

void CSubsystemDominator::SetRenderingSubsystem(CRenderingSubsystem* _RenderingSubsystem)
{
	m_RenderingSubsystem = _RenderingSubsystem;
}

void CSubsystemDominator::SetActorSubsystem(CActorSubsystem* _ActorSubsystem)
{
	m_ActorSubsystem = _ActorSubsystem;
}

void CSubsystemDominator::SetCameraSubsystem(CCameraSubsystem* _CameraSubsystem)
{
	m_CameraSubsystem = _CameraSubsystem;
}

void CSubsystemDominator::SetLightSubsystem(CLightSubsystem* _LightSubsystem)
{
	m_LightSubsystem = _LightSubsystem;
}
#endif