#pragma once

#include <Bifrost/Subsystem/SubsystemBase/SubsystemInitializerBase.h>

BIFROST_NAMESPACE_BEGIN

struct FUpdateIntervalSubsystemInitializer : public FSubsystemInitializerBase
{
	uint16 FramePerSecond = 0;
};

BIFROST_NAMESPACE_END