#include "CommandQueue.h"

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>


using namespace std;

USING_ATLANTIS;
USING_GLUE;

bool CCommandQueue::Initialize(const FCommandQueueInitializer& _Initializer)
{
	do
	{
		if (!_Initializer.Device) { break; }

		if (!CreateCommandQueue(_Initializer)) { break; }
	
		return true;

	} while (0);

	Finalize();
	return false;
}

void CCommandQueue::Finalize()
{
	ReleaseD3DPtr(m_CmdQueue);
}

bool CCommandQueue::CreateCommandQueue(const FCommandQueueInitializer& _Initializer)
{
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Flags = GetD3DCommandQueueFlag(_Initializer.QueueFlag);
	desc.NodeMask = _Initializer.NodeMask;
	desc.Priority = GetD3DCommandQueuePriority(_Initializer.QueuePriority);
	desc.Type = GetD3DCommandListType(_Initializer.listType);


	ID3D12CommandQueue* queue = nullptr;
	D3D_ERROR_CHECK(_Initializer.Device->CreateCommandQueue(
		&desc,
		IID_PPV_ARGS(&queue)));

	m_CmdQueue = move(unique_ptr<ID3D12CommandQueue>(queue));

	return true;
}