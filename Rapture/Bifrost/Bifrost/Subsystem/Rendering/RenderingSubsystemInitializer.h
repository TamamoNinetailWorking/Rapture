#pragma once

#include <Atlantis/DirectX12/DirectXPaste.h>
#include <Bifrost/Subsystem/SubsystemBase/SubsystemInitializerBase.h>

BIFROST_NAMESPACE_BEGIN

struct FRenderingSubsystemInitializer : public FSubsystemInitializerBase
{
	ATLANTIS_NAMESPACE::Glue::ECommandListType CommandListType = {};

	ATLANTIS_NAMESPACE::Glue::ECommandQueueFlag CommandQueueFlag = {};
	ATLANTIS_NAMESPACE::Glue::ECommandQueuePriority CommandQueuePriority = {};

	ATLANTIS_NAMESPACE::Glue::EResourceBarrierType BarrierType = {};

	uint32 ViewWidth = 0;
	uint32 ViewHeight = 0;
	HWND WindowHandle = {};

	ATLANTIS_NAMESPACE::Glue::EDataFormat RTVFormat = {};
	ATLANTIS_NAMESPACE::Glue::ERTVDimension RTVDimension = {};

	float FovAngle = 0.f;
	float NearZ = 0.f;
	float FarZ = 0.f;

	uint32 QueueReserveNum = 0;	
};

BIFROST_NAMESPACE_END