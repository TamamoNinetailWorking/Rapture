#pragma once

#include <memory>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <eden/include/template/NonCopyable.h>

ATLANTIS_NAMESPACE_BEGIN

/*
* DirectX12�̊�N���X
*/
class CDX12MainDevice : public CNonCopyable
{
public:

	// ������
	bool Initialize();

	// �I������
	void Finalize();

	// �f�o�b�O���C���[�̗L����
	bool EnableDebugLayer() const;


	// �Q�b�^�[
	//const ID3D12Device* GetDevice() const { return m_Device.get(); };
	//ID3D12Device* GetDeviceEdit() const { return m_Device.get(); };
	ID3D12Device* GetDevice() const { return m_Device.get(); };
	//const IDXGIFactory6* GetGIFactory() const { return m_GIFactory.get(); };
	//IDXGIFactory6* GetGIFactoryEdit() const { return m_GIFactory.get(); };
	IDXGIFactory6* GetGIFactory() const { return m_GIFactory.get(); };

	CDX12MainDevice() {};
	~CDX12MainDevice() {};

private:

	// GIFactory�̐���
	bool CreateGIFacotry();

	// DirectX�f�o�C�X�̐���
	bool CreateDevice();

	// �O���t�B�b�N�A�_�v�^�[�̎擾
	bool GetAdapter();


	// ���C���ɂȂ�DirectX�f�o�C�X
	std::unique_ptr<ID3D12Device> m_Device = nullptr;

	// GIFactory
	std::unique_ptr<IDXGIFactory6> m_GIFactory = nullptr;

	// �O���t�B�b�N�A�_�v�^�[
	std::unique_ptr<IDXGIAdapter> m_Adapter = nullptr;

	// �t�B�[�`���[���x��
	D3D_FEATURE_LEVEL m_FeatureLevel = {};

};


ATLANTIS_NAMESPACE_END