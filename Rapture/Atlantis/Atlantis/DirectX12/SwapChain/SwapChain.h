#pragma once

#include <memory>

#include <d3d12.h>
#include <dxgi1_6.h>

#include <Atlantis/DirectX12/RenderTargetView/RenderTargetView.h>

struct ID3D12Device;
struct ID3D12CommandQueue;
struct IDXGIFactory6;
struct IDXGISwapChain4;
struct D3D12_RESOURCE_BARRIER;
struct D3D12_RENDER_TARGET_VIEW_DESC;
struct ID3D12DescriptorHeap;

ATLANTIS_NAMESPACE_BEGIN

class CRenderTargetView;
class CCommandContext;
class CDX12MainDevice;

class CSwapChain
{
public:

	// �ʓ|�Ȃ̂ň�U���������B���͕K�v�ɂȂ�����ϐ������Ă�����
	struct FSwapChainInitializer
	{
		ID3D12Device* Device = nullptr;
		IDXGIFactory6* Factory = nullptr;
		ID3D12CommandQueue* Queue = nullptr;
		HWND WindowHandle = {};
		D3D12_RENDER_TARGET_VIEW_DESC* RtvDesc = nullptr;
		uint32 ViewportWidth = 0;
		uint32 ViewportHeight = 0;
		uint32 BufferCount = 2;
	};

	// ������
	bool Initialize(const FSwapChainInitializer& _Initializer);

	// �I������
	void Finalize();

	// �i�ꎞ�I�j�[�x�o�b�t�@�̐ݒ�
	void SetDepthBuffer(ID3D12DescriptorHeap* _DepthBuffer);

	// �`�揀��
	void Begin(CDX12MainDevice* _Device, CCommandContext* _Command);

	void End(CCommandContext* _Command);

	// �����_�[�^�[�Q�b�g�r���[�̃Z�b�g
	void SetRenderTargetView(CDX12MainDevice* _Device,CCommandContext* _Command);


	// �t���b�v
	void Present();


	// �Q�b�^�[
	IDXGISwapChain4* GetSwapChain() const { return m_SwapChain.get(); };

	const D3D12_CPU_DESCRIPTOR_HANDLE& GetRenderTargetViewHandle() const { return m_Handle; };


	CSwapChain() {};
	~CSwapChain() {};

private:

	bool CreateSwapChain(const FSwapChainInitializer& _Initializer);

	bool CreateRenderTargetView(const FSwapChainInitializer& _Initializer);

	bool CreateResourceBarrier();

	// �X���b�v�`�F�[��
	std::unique_ptr<IDXGISwapChain4> m_SwapChain = nullptr;

	// �����_�[�^�[�Q�b�g�r���[
	std::unique_ptr<CRenderTargetView> m_RenderTargetView = nullptr;

	// �����_�[�^�[�Q�b�g�̃��\�[�X�o���A
	std::unique_ptr<D3D12_RESOURCE_BARRIER> m_Barrier = nullptr;

	// �q�[�v�n���h��
	D3D12_CPU_DESCRIPTOR_HANDLE m_Handle = {};

	// �[�x�o�b�t�@
	std::unique_ptr<ID3D12DescriptorHeap> m_DepthBuffer = nullptr;

};


ATLANTIS_NAMESPACE_END