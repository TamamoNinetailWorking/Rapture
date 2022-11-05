#pragma once

struct D3D12_GRAPHICS_PIPELINE_STATE_DESC;
struct ID3D12PipelineState;

ATLANTIS_NAMESPACE_BEGIN

class CDX12MainDevice;
class CRootSignature;
class CGraphicsShader;
class CVertexShader;
class CHullShader;
class CDomainShader;
class CGeometoryShader;
class CPixelShader;

class CGraphicsPipeline
{
public:

	bool Initialize();
	void Finalize();

	// �p�C�v���C���X�e�[�g���Đ���
	bool RecreateState(CDX12MainDevice* _device);

	
	// ���[�g�V�O�l�`����ݒ�
	void SetRootSignature(const CRootSignature* _RootSignature);


	// -- �V�F�[�_�[ -- //
	// ���_�V�F�[�_�[��ݒ�
	void SetVertexShader(const CVertexShader* _Shader);

	// �n���V�F�[�_�[��ݒ�
	void SetHullShader(const CHullShader* _Shader);

	// �h���C���V�F�[�_�[��ݒ�
	void SetDomainShader(const CDomainShader* _Shader);

	// �W�I���g���V�F�[�_�[��ݒ�
	void SetGeometoryShader(const CGeometoryShader* _Shader);

	// �s�N�Z���V�F�[�_�[��ݒ�
	void SetPixelShader(const CPixelShader* _Shader);
	// ---------------- //


	// �Q�b�^�[
	ID3D12PipelineState* GetPipelineState() const { return m_PipelineState.get(); };
	D3D12_GRAPHICS_PIPELINE_STATE_DESC* GetPipelineDescEdit() const { return m_PipelineDesc.get(); };

	CGraphicsPipeline() {};
	~CGraphicsPipeline();

private:

	bool CreatePipelineDesc();

	void SetPipelineDescParameter();

	bool CreatePipelineState(CDX12MainDevice* _device);

	// �p�C�v���C���̍\���v�f
	std::unique_ptr<D3D12_GRAPHICS_PIPELINE_STATE_DESC> m_PipelineDesc = nullptr;

	// �p�C�v���C���X�e�[�g
	std::unique_ptr<ID3D12PipelineState> m_PipelineState = nullptr;
};


ATLANTIS_NAMESPACE_END