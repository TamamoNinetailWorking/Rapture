#pragma once

#include <memory>
#include <d3d12.h>

#include <Atlantis/DirectX12/DirectXPaste.h>

ATLANTIS_NAMESPACE_BEGIN

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


	ID3D12CommandQueue* GetCommandQueue() const { return m_CmdQueue.get(); };

	CCommandQueue() {};
	~CCommandQueue() {};

private:

	bool CreateCommandQueue(const FCommandQueueInitializer& _Initializer);


	// コマンドキュー
	std::unique_ptr<ID3D12CommandQueue> m_CmdQueue = nullptr;
};


ATLANTIS_NAMESPACE_END