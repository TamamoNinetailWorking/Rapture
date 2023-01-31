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

void CSubsystemDominator::SetUpdaterSubsystem(CUpdateProcessorSubsystem* _UpdaterSubsystem)
{
	m_UpdaterSubsystem = _UpdaterSubsystem;
}

void CSubsystemDominator::SetRenderingSubsystem(CRenderingSubsystem* _RenderingSubsystem)
{
	m_RenderingSubsystem = _RenderingSubsystem;
}
