#pragma once

#include <vector>
#include <memory>

#include <d3d12.h>

#include <Atlantis/DirectX12/MainDevice/MainDevice.h>
#include <Atlantis/DirectX12/Command/CommandContext.h>
#include <Atlantis/DirectX12/Command/CommandQueue.h>

ATLANTIS_NAMESPACE_BEGIN

class CDX12MainDevice;
class CCommandContext;
class CCommandQueue;

ATLANTIS_NAMESPACE_END

/*
*	�Q�[�������𓝊�����}�l�[�W���[
*/
class CGameManager
{
public:

	b8 Initialize(struct FGameManagerInitializer* _Initializer);

	void Finalize();

	void GameMain();

private:

	void GameUpdate();

	void Render();


	std::unique_ptr<ATLANTIS_NAMESPACE::CDX12MainDevice> m_MainDevice = nullptr;
	std::unique_ptr<ATLANTIS_NAMESPACE::CCommandContext> m_CommandContext = nullptr;
	std::unique_ptr<ATLANTIS_NAMESPACE::CCommandQueue> m_CommandQueue = nullptr;


	//// ������1�̂͂�
	//struct ID3D12Device* m_Device = nullptr;

	//// �R�}���h���X�g����������ꍇ�͂��ꂼ��K�v
	//struct ID3D12CommandAllocator* m_CmdAllocator = nullptr;
	//struct ID3D12GraphicsCommandList* m_CmdList = nullptr;
	//struct ID3D12CommandQueue* m_CmdQueue = nullptr;

	// �f�B�X�N���v�^�q�[�v�̓f�B�X�N���v�^�̐����݂���B
	// >>����̗p�r�ł����ƁARTV���ɂ����Ă��ǂ������B
	struct ID3D12DescriptorHeap* m_RtvHeaps = nullptr;

	// �t�F���X��GPU�̏����������������ǂ��������邱�Ƃ��ł���
	// >>�����͂܂�P�`��ɂP�Ή�����`���낤����A
	// >> �����ɕ`�悵�Ȃ�����͕����Ȃ��Ă��ǂ������B
	struct ID3D12Fence* m_Fence = nullptr;


	// ���b�V���P�ʂŎ�����
	struct ID3D12Resource* m_VertexBuffer = nullptr;
	struct D3D12_VERTEX_BUFFER_VIEW* m_VertexBufferView = nullptr;

	struct ID3D12Resource* m_IndexBuffer = nullptr;
	struct D3D12_INDEX_BUFFER_VIEW* m_IndexBufferView = nullptr;
	// -���b�V���P�ʂŎ�����

	// �O���t�B�b�N�p�C�v���C��
	struct D3D12_GRAPHICS_PIPELINE_STATE_DESC* m_PipeLine = nullptr;
	struct ID3D12PipelineState* m_PipeLineState = nullptr;

	// ���[�g�V�O�l�`��
	struct ID3D12RootSignature* m_RootSignature = nullptr;

	// �r���[�|�[�g
	struct D3D12_VIEWPORT* m_Viewport = nullptr;

	// �؂蔲����`
	D3D12_RECT m_ScissorRect = {};

	//// GIFactory��Device�Ɠ����悤�ɑ��̃I�u�W�F�N�g�𐶂ݏo���̂Ɏg���̂�
	//// 1�ŗǂ��������B
	//struct IDXGIFactory6* m_GIFactory = nullptr;

	// �X���b�v�`�F�[���͉�ʂ̐؂�ւ����Ɏg�����̂Ȃ̂ŁA
	// ������1�ŗǂ�����
	struct IDXGISwapChain4* m_SwapChain = nullptr;


	// BackBuffer��RTV�Ɏg���Ă���̂ŁA����ƃZ�b�g���낤
	std::vector<struct ID3D12Resource*> m_BackBuffers;

	// �t�F���X�ƃZ�b�g
	u64 m_FenceValue = 0;

};