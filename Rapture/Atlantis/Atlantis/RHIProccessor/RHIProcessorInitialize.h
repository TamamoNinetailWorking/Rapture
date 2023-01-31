#pragma once

#include <Atlantis/DirectX12/DirectXPaste.h>

ATLANTIS_NAMESPACE_BEGIN

struct FRHIProcessorInitialize
{
	Glue::ECommandListType CommandListType = {};

	Glue::ECommandQueueFlag CommandQueueFlag = {};
	Glue::ECommandQueuePriority CommandQueuePriority = {};

	Glue::EResourceBarrierType BarrierType = {};
};

ATLANTIS_NAMESPACE_END