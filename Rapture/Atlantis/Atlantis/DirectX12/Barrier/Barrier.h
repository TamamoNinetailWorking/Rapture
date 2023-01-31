#pragma once

#include <Atlantis/DirectX12/DirectXPaste.h>

struct D3D12_RESOURCE_BARRIER;

ATLANTIS_NAMESPACE_BEGIN

class CBarrier
{
public:

	struct FInitializer
	{
		Glue::EResourceBarrierType BarrierType = {};
	};

	struct FTransitionState
	{
		struct ID3D12Resource* Resource = nullptr;
		Glue::EResourceState Before = {};
		Glue::EResourceState After = {};
	};

	bool Initializer(const FInitializer& _Initializer);
	
	void Finalize();

	void SetTransitionState(const FTransitionState& _state);

	void SwapTransitionState();

	D3D12_RESOURCE_BARRIER* GetBarrier() const { return m_Barrier; };

	CBarrier() {};
	~CBarrier() {};

private:

	bool CreateResourceBarrier(const FInitializer& _Initializer);

	ObjectPtr(D3D12_RESOURCE_BARRIER) m_Barrier = nullptr;

};


ATLANTIS_NAMESPACE_END