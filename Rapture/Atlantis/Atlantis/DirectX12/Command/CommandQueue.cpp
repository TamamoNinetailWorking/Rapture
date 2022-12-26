#include "CommandQueue.h"

#include <vector>

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>
#include <Atlantis/DirectX12/Fence/Fence.h>
#include <Atlantis/DirectX12/Command/CommandContext.h>


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
	//ReleaseD3DUniquePtr(m_CmdQueue);
	SafeReleaseD3DPtr(m_CmdQueue);
}

void CCommandQueue::Signal(CFence* _Fence)
{
	if (!_Fence) { return; }

	m_CmdQueue->Signal(_Fence->GetFence(), _Fence->GetFenceValueAndIncrement());
}

void CCommandQueue::CommandListPush(CCommandContext* _List)
{
	if (!_List) { return; }
	m_ListQueue.push_back(_List);
}

void CCommandQueue::Execute()
{
	if (m_ListQueue.empty()) { return; }
	uint32 size = SCast<uint32>(m_ListQueue.size());
	vector<ID3D12CommandList*> vec(size);
	uint32 index = 0;
	for (auto& elem : m_ListQueue)
	{
		ID3D12CommandList* list = elem->GetCommandList();
		vec[index++] = list;
	}

	m_CmdQueue->ExecuteCommandLists(size, vec.data());
	m_ListQueue.clear();
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

	//m_CmdQueue = move(unique_ptr<ID3D12CommandQueue>(queue));
	m_CmdQueue = queue;

	return true;
}