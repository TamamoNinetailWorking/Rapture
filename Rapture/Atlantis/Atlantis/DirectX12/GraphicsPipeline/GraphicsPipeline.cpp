#include "GraphicsPipeline.h"

#include <d3d12.h>
#include <dxgi1_6.h>

#include <eden/include/utility/ender_utility.h>
#include <Atlantis/DirectX12/DirectX12BaseDefine.h>

#include <Atlantis/DirectX12/MainDevice/MainDevice.h>
#include <Atlantis/DirectX12/RootSignature/RootSignature.h>
#include <Atlantis/DirectX12/Shader/GraphicsShader.h>

EDENS_NAMESPACE_USING;
USING_ATLANTIS;


bool CGraphicsPipeline::Initialize()
{
	do
	{
		CHECK_RESULT_BREAK(CreatePipelineDesc());

		return true;

	} while (0);

	Finalize();
	return false;
}

void CGraphicsPipeline::Finalize()
{
	//ReleaseD3DUniquePtr(m_PipelineState);
	SafeReleaseD3DPtr(m_PipelineState);
	if (m_PipelineDesc)
	{
		delete m_PipelineDesc;
		m_PipelineDesc = nullptr;
	}
}

bool CGraphicsPipeline::RecreateState(CDX12MainDevice* _device)
{
	CHECK_RESULT_FALSE(_device);
	if (m_PipelineState)
	{
		// この処理を一度パイプラインをセットした後に行うとまずそう
		// >> 恐らく保持されている参照カウンタが0にならずに未開放のまま再作成しようとしてエラーになる
		// unique_ptrを使わないようにしておく
		// >>これをvector配列などで持たないとは限らないため
		uint32 result = m_PipelineState->Release();
		//m_PipelineState.release();
	}
	return CreatePipelineState(_device);
}

void CGraphicsPipeline::SetRootSignature(const CRootSignature* _RootSignature)
{
	D3D_CHECK(_RootSignature);
	m_PipelineDesc->pRootSignature = _RootSignature->GetRootSignature();
}

void CGraphicsPipeline::SetVertexShader(const CVertexShader* _Shader)
{
	D3D_CHECK(_Shader);
	m_PipelineDesc->VS.pShaderBytecode = _Shader->GetShaderByteCode();
	m_PipelineDesc->VS.BytecodeLength = _Shader->GetBufferSize();

	m_PipelineDesc->InputLayout.pInputElementDescs = _Shader->GetInputLayout();
	m_PipelineDesc->InputLayout.NumElements = _Shader->GetInputNum();
}

void CGraphicsPipeline::SetHullShader(const CHullShader* _Shader)
{
	D3D_CHECK(_Shader);
	m_PipelineDesc->HS.pShaderBytecode = _Shader->GetShaderByteCode();
	m_PipelineDesc->HS.BytecodeLength = _Shader->GetBufferSize();
}

void CGraphicsPipeline::SetDomainShader(const CDomainShader* _Shader)
{
	D3D_CHECK(_Shader);
	m_PipelineDesc->DS.pShaderBytecode = _Shader->GetShaderByteCode();
	m_PipelineDesc->DS.BytecodeLength = _Shader->GetBufferSize();
}

void CGraphicsPipeline::SetGeometoryShader(const CGeometryShader* _Shader)
{
	D3D_CHECK(_Shader);
	m_PipelineDesc->GS.pShaderBytecode = _Shader->GetShaderByteCode();
	m_PipelineDesc->GS.BytecodeLength = _Shader->GetBufferSize();
}

void CGraphicsPipeline::SetPixelShader(const CPixelShader* _Shader)
{
	D3D_CHECK(_Shader);
	m_PipelineDesc->PS.pShaderBytecode = _Shader->GetShaderByteCode();
	m_PipelineDesc->PS.BytecodeLength = _Shader->GetBufferSize();
}

CGraphicsPipeline::~CGraphicsPipeline()
{
}

bool CGraphicsPipeline::CreatePipelineDesc()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC* desc = new D3D12_GRAPHICS_PIPELINE_STATE_DESC();
	CHECK_RESULT_FALSE(desc);

	desc->pRootSignature = nullptr;

	//m_PipelineDesc.reset(desc);
	m_PipelineDesc = desc;

	SetPipelineDescParameter();

	return true;
}

void CGraphicsPipeline::SetPipelineDescParameter()
{
	m_PipelineDesc->SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	m_PipelineDesc->IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	m_PipelineDesc->PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	m_PipelineDesc->SampleDesc.Count = 1;
	m_PipelineDesc->SampleDesc.Quality = 0;
}

bool CGraphicsPipeline::CreatePipelineState(CDX12MainDevice* _device)
{
	CHECK_RESULT_FALSE(_device);
	CHECK_RESULT_FALSE(m_PipelineDesc);
	CHECK_RESULT_FALSE(m_PipelineDesc->pRootSignature);

	ID3D12PipelineState* pipelineState = nullptr;
	D3D_ERROR_CHECK(_device->GetDevice()->CreateGraphicsPipelineState(
		m_PipelineDesc, 
		IID_PPV_ARGS(&pipelineState)));

	//m_PipelineState.reset(pipelineState);
	m_PipelineState = pipelineState;

	return true;
}