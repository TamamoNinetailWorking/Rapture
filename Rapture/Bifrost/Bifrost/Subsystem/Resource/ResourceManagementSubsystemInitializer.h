#pragma once

#include <Bifrost/Subsystem/SubsystemBase/SubsystemInitializerBase.h>

BIFROST_NAMESPACE_BEGIN

struct FResourceManagementSubsystemInitializer : public FSubsystemInitializerBase
{
	uint32 ResourceTypeNum = 0;
};

BIFROST_NAMESPACE_END