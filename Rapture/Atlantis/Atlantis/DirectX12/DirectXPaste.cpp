#include "DirectXPaste.h"


ATLANTIS_NAMESPACE_BEGIN

namespace Glue
{
	const D3D12_COMMAND_LIST_TYPE GetD3DCommandListType(ECommandListType _ListType)
	{
		return SCast<D3D12_COMMAND_LIST_TYPE>(_ListType);
	}

	const D3D12_COMMAND_QUEUE_FLAGS GetD3DCommandQueueFlag(ECommandQueueFlag _Flag)
	{
		return SCast<D3D12_COMMAND_QUEUE_FLAGS>(_Flag);
	}

	const D3D12_COMMAND_QUEUE_PRIORITY GetD3DCommandQueuePriority(ECommandQueuePriority _Priority)
	{
		return SCast<D3D12_COMMAND_QUEUE_PRIORITY>(_Priority);
	}
};

ATLANTIS_NAMESPACE_END