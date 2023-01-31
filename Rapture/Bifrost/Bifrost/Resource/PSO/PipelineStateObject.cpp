#include "PipelineStateObject.h"
#include "PipelineStateObjectInitializer.h"

#include <eden/include/utility/ender_utility.h>

#include <Atlantis/DirectX12/GraphicsPipeline/GraphicsPipeline.h>
#include <Atlantis/DirectX12/RootSignature/RootSignature.h>

#include <eden/include/Resource/ResourceManager.h>
#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Resource/ResourceSubsystemInterface.h>

#include <Atlantis/DirectX12/Shader/ShaderBaseInitializer.h>
#include <Atlantis/DirectX12/Shader/GraphicsShader.h>

#include <Atlantis/DirectX12/MainDevice/MainDevice.h>


USING_BIFROST;
USING_ATLANTIS;
EDENS_NAMESPACE_USING;

bool CPipelineStateObject::Initialize(const EDENS_NAMESPACE::FResourceInitializerBase* _Initializer)
{
	do
	{
		const FPipelineStateObjectInitializer* initializer = PCast<const FPipelineStateObjectInitializer*>(_Initializer);
		CHECK_RESULT_BREAK(initializer);

		m_ResourceName = initializer->Name;

		CHECK_RESULT_BREAK(CreateRootSignature(initializer));

		CHECK_RESULT_BREAK(CreatePipeline(initializer));

		return true;
	} while (0);

	Finalize();
	return false;
}

void CPipelineStateObject::Finalize()
{
	EDENS_NAMESPACE::FinalizeObject(m_RootSignature);
	EDENS_NAMESPACE::FinalizeObject(m_Pipeline);

	ReleaseShader();
}

bool CPipelineStateObject::CreateRootSignature(const FPipelineStateObjectInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(_Initializer->Device);

	m_RootSignature = new CRootSignature();
	CHECK_RESULT_FALSE(m_RootSignature);

	CRootSignature::FInitializer initializer = {};
	initializer.Device = _Initializer->Device->GetDevice();
	initializer.Flag = _Initializer->Flag;
	initializer.pParameters = _Initializer->pParameters;
	initializer.NumParameters = _Initializer->NumParameters;
	initializer.pStaticSamplers = _Initializer->pStaticSamplers;
	initializer.NumStaticSamplers = _Initializer->NumStaticSamplers;

	return m_RootSignature->Initialize(initializer);
}

void CPipelineStateObject::ReleaseShader()
{
	CResourceManager* manager = CSubsystemServiceLocator::GetResourceSubsystemEdit()->GetShaderResourceManagerEdit();
	if (!manager) { return; }

	manager->DeleteResource(m_PSHandle);
	manager->DeleteResource(m_GSHandle);
	manager->DeleteResource(m_DSHandle);
	manager->DeleteResource(m_HSHandle);
	manager->DeleteResource(m_VSHandle);
}

bool CPipelineStateObject::CreatePipeline(const FPipelineStateObjectInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	m_Pipeline = new CGraphicsPipeline();
	CHECK_RESULT_FALSE(m_Pipeline);

	CHECK_RESULT_FALSE(m_Pipeline->Initialize());

	D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc = *m_Pipeline->GetPipelineDescEdit();
	desc = *_Initializer->pPipelineDesc;

	m_Pipeline->SetRootSignature(m_RootSignature);

	CResourceManager* manager = CSubsystemServiceLocator::GetResourceSubsystemEdit()->GetShaderResourceManagerEdit();

	{
		m_VSHandle = manager->SearchCreateResource(_Initializer->VertexShaderInit);
		if (manager->IsValidHandle(m_VSHandle))
		{
			const CVertexShader* pVS = PCast<const CVertexShader*>(manager->SearchResource(m_VSHandle));
			m_Pipeline->SetVertexShader(pVS);
		}

		m_DSHandle = manager->SearchCreateResource(_Initializer->DomainShaderInit);
		if (manager->IsValidHandle(m_DSHandle))
		{
			const CDomainShader* pDS = PCast<const CDomainShader*>(manager->SearchResource(m_DSHandle));
			m_Pipeline->SetDomainShader(pDS);
		}

		m_HSHandle = manager->SearchCreateResource(_Initializer->HullShaderInit);
		if (manager->IsValidHandle(m_HSHandle))
		{
			const CHullShader* pHS = PCast<const CHullShader*>(manager->SearchResource(m_HSHandle));
			m_Pipeline->SetHullShader(pHS);
		}

		m_GSHandle = manager->SearchCreateResource(_Initializer->GeometryShaderInit);
		if (manager->IsValidHandle(m_GSHandle))
		{
			const CGeometryShader* pGS = PCast<const CGeometryShader*>(manager->SearchResource(m_GSHandle));
			m_Pipeline->SetGeometoryShader(pGS);
		}

		m_PSHandle = manager->SearchCreateResource(_Initializer->PixelShaderInit);
		if (manager->IsValidHandle(m_PSHandle))
		{
			const CPixelShader* pPS = PCast<const CPixelShader*>(manager->SearchResource(m_PSHandle));
			m_Pipeline->SetPixelShader(pPS);
		}
	}


	return m_Pipeline->RecreateState(_Initializer->Device);
}
