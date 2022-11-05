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

	// �����҂��̃C�x���g���s
	void WaitEvent();


	ID3D12Fence* GetFence() const { return m_Fence.get(); };
	uint64 GetFenceValue() const { return m_FenceValue; };

	CFence() {};
	~CFence() {};

private:


	bool CreateFence(const FFenceInitializer& _Initializer);

	// �t�F���X
	std::unique_ptr<ID3D12Fence> m_Fence = nullptr;

	// �t�F���X�o�����[
	uint64 m_FenceValue = 0;
};


ATLANTIS_NAMESPACE_END