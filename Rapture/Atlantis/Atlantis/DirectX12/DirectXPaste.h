#pragma once

#include <d3d12.h>
#include <DirectXmath.h>

#define USING_GLUE using namespace ATLANTIS_NAMESPACE::Glue;

ATLANTIS_NAMESPACE_BEGIN

// DirectXì¡óLÇÃÇ‡ÇÃÇ©ÇÁó\ÇﬂíËã`ÇµÇΩÇ‡ÇÃÇ…íuÇ´ä∑Ç¶ÇƒÇ¢Ç≠
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



// éZèpââéZån
namespace Glue
{

	typedef DirectX::XMFLOAT3 Vector2;
	typedef DirectX::XMFLOAT3 Vector3;
	typedef DirectX::XMFLOAT4 Vector4;


	typedef DirectX::XMFLOAT3X4 Matrix34;
	typedef DirectX::XMFLOAT4X3 Matrix43;
	typedef DirectX::XMFLOAT4X4 Matrix44;

};


ATLANTIS_NAMESPACE_END