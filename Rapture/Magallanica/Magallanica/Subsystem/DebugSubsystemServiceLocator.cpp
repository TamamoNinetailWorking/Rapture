#include "DebugSubsystemServiceLocator.h"

USING_MAGALLANICA;

CDebugWindowSubsystem* CDebugSubsystemServiceLocator::m_DebugWindowSubsystem = nullptr;

void CDebugSubsystemServiceLocator::SetDebugWindowSubsystem(CDebugWindowSubsystem* _Subsystem)
{
	m_DebugWindowSubsystem = _Subsystem;
}

CDebugWindowSubsystem* CDebugSubsystemServiceLocator::GetDebugWindowSubsystem()
{
	return m_DebugWindowSubsystem;
}
