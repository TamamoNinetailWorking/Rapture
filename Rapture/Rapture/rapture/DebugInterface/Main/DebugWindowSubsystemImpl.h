#pragma once

#include <Magallanica/Subsystem/DebugWindowSubsystem.h>

class CDebugWindowSubsystemImpl : public MAGALLANICA_NAMESPACE::CDebugWindowSubsystem
{
protected:

	bool InitializeImpl() override;

};