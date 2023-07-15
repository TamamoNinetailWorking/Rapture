#pragma once

MAGALLANICA_NAMESPACE_BEGIN

class CDebugWindowSubsystem;

class CDebugSubsystemServiceLocator
{
public:

	static void SetDebugWindowSubsystem(CDebugWindowSubsystem* _Subsystem);

	static CDebugWindowSubsystem* GetDebugWindowSubsystem();

private:

	static CDebugWindowSubsystem* m_DebugWindowSubsystem;

};

MAGALLANICA_NAMESPACE_END