#pragma once

#include <memory>
#include <deque>

#include <Atlantis/DirectX12/DirectXPaste.h>

ATLANTIS_NAMESPACE_BEGIN

class CCommandContext;

class CCommandQueue
{
public:

	struct FCommandQueueInitializer
	{
		ID3D12Device* Device = nullptr;
		uint32 NodeMask = 0;
		Glue::ECommandQueueFlag QueueFlag;
		Glue::ECommandQueuePriority QueuePriority;
		Glue::ECommandListType listType;
	};


	bool Initialize(const FCommandQueueInitializer& _Initializer);
	void Finalize();

	void Signal(class CFence* _Fence);

	void CommandListPush(CCommandContext* _List);

	void Execute();


	ID3D12CommandQueue* GetCommandQueue() const { return m_CmdQueue; };

	CCommandQueue() {};
	~CCommandQueue() {};

private:

	bool CreateCommandQueue(const FCommandQueueInitializer& _Initializer);


	// コマンドキュー
	//std::unique_ptr<ID3D12CommandQueue> m_CmdQueue = nullptr;
	ObjectPtr(ID3D12CommandQueue) m_CmdQueue = nullptr;

	// コマンドリストのキュー
	std::deque<CCommandContext*> m_ListQueue = {};
};


ATLANTIS_NAMESPACE_END