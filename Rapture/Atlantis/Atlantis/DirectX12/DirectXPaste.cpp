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

	const D3D12_ROOT_SIGNATURE_FLAGS GetD3DRootSignatureFlag(ERootSignatureFlag _Flag)
	{
		return SCast<D3D12_ROOT_SIGNATURE_FLAGS>(_Flag);
	}

	const D3D12_RESOURCE_BARRIER_TYPE GetD3DResourceBarrierType(EResourceBarrierType _Type)
	{
		return SCast<D3D12_RESOURCE_BARRIER_TYPE>(_Type);
	}

	const D3D12_RESOURCE_STATES GetD3DResourceState(EResourceState _state)
	{
		return  SCast<D3D12_RESOURCE_STATES>(_state);
	}
};

ATLANTIS_NAMESPACE_END