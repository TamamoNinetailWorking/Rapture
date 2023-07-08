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

	const D3D12_RESOURCE_DIMENSION GetD3DResourceDimension(EResourceDimension _dimension)
	{
		return SCast<D3D12_RESOURCE_DIMENSION>(_dimension);
	}

	const D3D12_RTV_DIMENSION GetD3DRTVDimension(ERTVDimension _Dimension)
	{
		return SCast<D3D12_RTV_DIMENSION>(_Dimension);
	}

	const DXGI_FORMAT GetDXGIFormat(EDataFormat _format)
	{
		return SCast<DXGI_FORMAT>(_format);
	}

	const D3D12_DESCRIPTOR_RANGE_TYPE GetRangeType(EDescriptorHeapRangeType _Type)
	{
		return SCast<D3D12_DESCRIPTOR_RANGE_TYPE>(_Type);
	}

	const D3D12_SHADER_VISIBILITY GetShaderVisibility(EShaderVisibility _Visibility)
	{
		return SCast<D3D12_SHADER_VISIBILITY>(_Visibility);
	}

	const D3D12_PRIMITIVE_TOPOLOGY GetPrimitiveTopology(EPrimitiveTopology _Topology)
	{
		return SCast<D3D12_PRIMITIVE_TOPOLOGY>(_Topology);
	}

	const D3D12_DESCRIPTOR_HEAP_TYPE GetDescriptorHeapType(EDescriptorHeapType _HeapType)
	{
		return SCast<D3D12_DESCRIPTOR_HEAP_TYPE>(_HeapType);
	}
};

ATLANTIS_NAMESPACE_END