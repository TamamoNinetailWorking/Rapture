#include "Fence.h"

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>

using namespace std;
USING_ATLANTIS;

bool CFence::Initialize(const FFenceInitializer& _Initializer)
{
	return CreateFence(_Initializer);
}

void CFence::Finalize()
{
	//ReleaseD3DUniquePtr(m_Fence);
	SafeReleaseD3DPtr(m_Fence);
}

void CFence::WaitEvent()
{
	if (m_Fence->GetCompletedValue() != m_FenceValue)
	{
		auto event = CreateEvent(nullptr, false, false, nullptr);
		m_Fence->SetEventOnCompletion(m_FenceValue, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
}

uint64 CFence::GetFenceValueAndIncrement()
{
	return ++m_FenceValue;
}

bool CFence::CreateFence(const FFenceInitializer& _Initializer)
{
	if (!_Initializer.Device) { return false; };

	ID3D12Fence* fence = nullptr;
	D3D_ERROR_CHECK(_Initializer.Device->CreateFence(
		m_FenceValue,
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&fence)));

	//m_Fence.reset(fence);
	m_Fence = fence;

	return true;
}
