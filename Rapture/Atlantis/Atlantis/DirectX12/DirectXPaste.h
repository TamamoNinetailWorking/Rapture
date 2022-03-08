#pragma once

#include <d3d12.h>

#define USING_GLUE using namespace ATLANTIS_NAMESPACE::Glue;

ATLANTIS_NAMESPACE_BEGIN

// DirectX“Á—L‚Ì‚à‚Ì‚©‚ç—\‚ß’è‹`‚µ‚½‚à‚Ì‚É’u‚«Š·‚¦‚Ä‚¢‚­
namespace Glue
{

	enum ECommandListType : uint8
	{
		COMMAND_LIST_TYPE_DIRECT = 0,
		COMMAND_LIST_TYPE_BUNDLE = 1,
		COMMAND_LIST_TYPE_COMPUTE = 2,
		COMMAND_LIST_TYPE_COPY = 3,
		COMMAND_LIST_TYPE_VIDEO_DECODE = 4,
		COMMAND_LIST_TYPE_VIDEO_PROCESS = 5,
		COMMAND_LIST_TYPE_VIDEO_ENCODE = 6
	};

	const D3D12_COMMAND_LIST_TYPE GetD3DCommandListType(ECommandListType _ListType);

	enum ECommandQueueFlag : uint8
	{
		COMMAND_QUEUE_FLAG_NONE = 0,
		COMMAND_QUEUE_FLAG_DISABLE_GPU_TIMEOUT = 0x1
	};

	const D3D12_COMMAND_QUEUE_FLAGS GetD3DCommandQueueFlag(ECommandQueueFlag _Flag);

	enum ECommandQueuePriority : uint32
	{
		COMMAND_QUEUE_PRIORITY_NORMAL = 0,
		COMMAND_QUEUE_PRIORITY_HIGH = 100,
		COMMAND_QUEUE_PRIORITY_GLOBAL_REALTIME = 10000
	};

	const D3D12_COMMAND_QUEUE_PRIORITY GetD3DCommandQueuePriority(ECommandQueuePriority _Priority);

};


ATLANTIS_NAMESPACE_END