#include "Barrier.h"

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>
#include <eden/include/utility/ender_utility.h>

USING_ATLANTIS;

using namespace Glue;

bool CBarrier::Initializer(const FInitializer& _initializer)
{
	do
	{
		D3D_INIT_PROCESS_CHECK(CreateResourceBarrier(_initializer));


		return true;
	} while (0);
	return false;
}

void CBarrier::Finalize()
{
	EDENS_NAMESPACE::Delete(m_Barrier);
}

void CBarrier::SetTransitionState(const FTransitionState& _state)
{
	m_Barrier->Transition.pResource = _state.Resource;
	m_Barrier->Transition.StateBefore = GetD3DResourceState(_state.Before);
	m_Barrier->Transition.StateAfter = GetD3DResourceState(_state.After);

}

void CBarrier::SwapTransitionState()
{
	D3D12_RESOURCE_STATES tempState = {};
	tempState = m_Barrier->Transition.StateBefore;
	m_Barrier->Transition.StateBefore = m_Barrier->Transition.StateAfter;
	m_Barrier->Transition.StateAfter = tempState;
}

bool CBarrier::CreateResourceBarrier(const FInitializer& _Initializer)
{
	D3D12_RESOURCE_BARRIER* desc = new D3D12_RESOURCE_BARRIER();
	D3D_CHECK_NULL(desc);

	desc->Type = GetD3DResourceBarrierType(_Initializer.BarrierType);
	desc->Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	switch(_Initializer.BarrierType)
	{
	case EResourceBarrierType::BARRIER_TYPE_TRANSITION:
		desc->Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		break;
	};
	
	m_Barrier = desc;

	return true;
}
