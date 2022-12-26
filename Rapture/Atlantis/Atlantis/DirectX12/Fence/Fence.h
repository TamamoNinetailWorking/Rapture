#pragma once

#include <memory>
#include <d3d12.h>

ATLANTIS_NAMESPACE_BEGIN

class CFence
{
public:
	
	struct FFenceInitializer
	{
		ID3D12Device* Device = nullptr;
	};

	bool Initialize(const FFenceInitializer& _Initializer);
	void Finalize();

	// 完了待ちのイベント発行
	void WaitEvent();


	ID3D12Fence* GetFence() const { return m_Fence; };
	uint64 GetFenceValue() const { return m_FenceValue; };
	uint64 GetFenceValueAndIncrement();

	CFence() {};
	~CFence() {};

private:


	bool CreateFence(const FFenceInitializer& _Initializer);

	// フェンス
	//std::unique_ptr<ID3D12Fence> m_Fence = nullptr;
	ObjectPtr(ID3D12Fence) m_Fence = nullptr;

	// フェンスバリュー
	uint64 m_FenceValue = 0;
};


ATLANTIS_NAMESPACE_END