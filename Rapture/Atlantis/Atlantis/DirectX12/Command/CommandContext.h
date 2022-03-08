#pragma once

#include <memory>
#include <d3d12.h>

#include <Atlantis/DirectX12/DirectXPaste.h>

ATLANTIS_NAMESPACE_BEGIN

class CCommandContext
{
public:

	struct FCommandContextInitializer
	{
		ID3D12Device* Device = nullptr;
		ID3D12PipelineState* PipelineState = nullptr;
		uint32 NodeMask = 0;
		Glue::ECommandListType listType;
	};

	bool Initialize(const FCommandContextInitializer& _Initializer);
	void Finalize();

	// ���Z�b�g
	void Reset(ID3D12PipelineState* _PipelineState = nullptr);

	// �Q�b�^�[
	ID3D12CommandAllocator* GetCommandAllocator() const { return m_CmdAllocator.get(); };
	ID3D12GraphicsCommandList* GetCommandList() const { return m_CmdList.get(); };



	CCommandContext() {};
	~CCommandContext() {};

private:

	bool CreateCommandAllocator(const FCommandContextInitializer& _Initializer);

	bool CreateCommandList(const FCommandContextInitializer& _Initializer);


	// �R�}���h�A���P�[�^�[�i�R�}���h�̎��̂�ۗL����j
	std::unique_ptr<ID3D12CommandAllocator> m_CmdAllocator = nullptr;

	// �R�}���h���X�g�i�R�}���h�̉��z���X�g�j
	std::unique_ptr<ID3D12GraphicsCommandList> m_CmdList = nullptr;

};


ATLANTIS_NAMESPACE_END