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

	// パイプラインステートを再生成
	bool RecreateState(CDX12MainDevice* _device);

	
	// ルートシグネチャを設定
	void SetRootSignature(const CRootSignature* _RootSignature);


	// -- シェーダー -- //
	// 頂点シェーダーを設定
	void SetVertexShader(const CVertexShader* _Shader);

	// ハルシェーダーを設定
	void SetHullShader(const CHullShader* _Shader);

	// ドメインシェーダーを設定
	void SetDomainShader(const CDomainShader* _Shader);

	// ジオメトリシェーダーを設定
	void SetGeometoryShader(const CGeometoryShader* _Shader);

	// ピクセルシェーダーを設定
	void SetPixelShader(const CPixelShader* _Shader);
	// ---------------- //


	// ゲッター
	ID3D12PipelineState* GetPipelineState() const { return m_PipelineState.get(); };
	D3D12_GRAPHICS_PIPELINE_STATE_DESC* GetPipelineDescEdit() const { return m_PipelineDesc.get(); };

	CGraphicsPipeline() {};
	~CGraphicsPipeline();

private:

	bool CreatePipelineDesc();

	void SetPipelineDescParameter();

	bool CreatePipelineState(CDX12MainDevice* _device);

	// パイプラインの構成要素
	std::unique_ptr<D3D12_GRAPHICS_PIPELINE_STATE_DESC> m_PipelineDesc = nullptr;

	// パイプラインステート
	std::unique_ptr<ID3D12PipelineState> m_PipelineState = nullptr;
};


ATLANTIS_NAMESPACE_END