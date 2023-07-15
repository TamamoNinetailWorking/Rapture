#include "BloomPostEffectComponent.h"
#include "BloomPostEffectComponentInitializer.h"

#include <Bifrost/RenderTarget/RenderTarget.h>
#include <Bifrost/RenderTarget/RenderTargetInitializer.h>

#include <Atlantis/DirectX12/RenderTargetView/RenderTargetView.h>
#include <Atlantis/DirectX12/RenderTargetView/RenderTargetViewResource.h>

#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Rendering/RenderingSubsystem.h>
#include <Atlantis/RHIProccessor/RHIProcessor.h>
#include <Atlantis/DirectX12/MainDevice/MainDevice.h>
#include <Atlantis/DirectX12/SwapChain/SwapChain.h>
#include <Atlantis/RHIProccessor/RHIRenderTargetView.h>

#include <eden/include/Resource/ResourceManager.h>
#include <Bifrost/Subsystem/Resource/ResourceSubsystemInterface.h>

#include <Bifrost/Model/MeshData/MeshData.h>
#include <Bifrost/Model/MeshData/MeshDataInitializer.h>
#include <Bifrost/GeometricPrimitive/Plain.h>

#include <Bifrost/Material/QuadPolygonMaterial.h>
#include <Bifrost/Material/QuadPolygonMaterialInitializer.h>

#include <Atlantis/Material/MaterialInterface.h>

#include <eden/include/utility/ender_utility.h>
#include <string>

USING_BIFROST;
USING_ATLANTIS;
EDENS_NAMESPACE_USING;

CBloomEffectComponent::CBloomEffectComponent() : Super()
{
	m_RenderGroup = ERenderGroup::RENDER_GROUP_POSTEFFECT;
}

bool CBloomEffectComponent::Initialize(const FComponentInitializerBase* _Initializer)
{
	do
	{ 
		CHECK_RESULT_FALSE(Super::Initialize(_Initializer));

		const FBloomPostEffectComponentInitializer* initializer = PCast<const FBloomPostEffectComponentInitializer*>(_Initializer);

		CHECK_RESULT_FALSE(CreateRTVBuffers());

		CHECK_RESULT_FALSE(CreateMeshData());
		CHECK_RESULT_FALSE(CreateMaterial(initializer));

		return true;
	} while (0);

	Finalize();
	return false;
}

void CBloomEffectComponent::Finalize()
{
	FinalizeObject(m_MaterialInterface);

	CResourceManager* Manager = CSubsystemServiceLocator::GetResourceSubsystemEdit()->GetMeshResourceManagerEdit();
	CHECK(Manager);

	Manager->DeleteResource(m_MeshData->GetName());
	m_MeshData = nullptr;

	for (auto& elem : m_RTVBuffers)
	{
		FinalizeObject(elem);
	}

	Super::Finalize();
}

bool CBloomEffectComponent::Draw() const
{
	CRenderingSubsystem* Subsystem = CSubsystemServiceLocator::GetRenderingSubsystemEdit();
	CHECK_RESULT_FALSE(Subsystem);

	CRenderTargetView* RTV = m_RTVBuffers.at(0)->GetRenderTargetView()->GetRenderTargetView();
	CHECK_RESULT_FALSE(RTV);
	Subsystem->SwitchDefaultRenderTargetViewBefore();
	
	{
		Subsystem->SwitchRenderTargetViewBefore(RTV);

		{
			Subsystem->SetMeshData(m_MeshData);

			auto& Material = m_MaterialInterface;
			Subsystem->SetMaterialInterface(m_MaterialInterface);

			//uint64 Handle = Subsystem->GetMaterialHeapHandle(Material);
			//auto DescHeap = Material->GetDescriptorHeap();
			//auto Handle = DescHeap->GetGPUDescriptorHandleForHeapStart().ptr;

			//Subsystem->SetGraphicsRootDescriptorTable(1, Handle);

			Subsystem->SetDefaultRenderTargetToSRV(0);
			Subsystem->DrawIndexedInstanced(CPrimitivePlain::GetIndicesNum(), 0);
		}

		Subsystem->SwitchRenderTargetViewAfter(RTV);
	}

	Subsystem->SwitchDefaultRenderTargetViewAfter();

	{
		Subsystem->SetMeshData(m_MeshData);

		auto& Material = m_MaterialInterface;
		Subsystem->SetMaterialInterface(m_MaterialInterface);
		
		auto Handle = RTV->GetShaderResourceView()->GetGPUDescriptorHandleForHeapStart().ptr;

		Subsystem->SetGraphicsRootDescriptorTable(0, Handle);
		Subsystem->DrawIndexedInstanced(CPrimitivePlain::GetIndicesNum(), 0);
	}

	return true;
}

bool CBloomEffectComponent::CreateRTVBuffers()
{
	const char* NameBase = "BloomRenderTargetView";

	CRenderingSubsystem* Subsystem = CSubsystemServiceLocator::GetRenderingSubsystemEdit();
	CHECK_RESULT_FALSE(Subsystem);
	const auto& SystemRTV = Subsystem->GetRTV();
	CHECK_RESULT_FALSE(SystemRTV);
	const auto& SwapChain = SystemRTV->GetSwapChain();
	CHECK_RESULT_FALSE(SwapChain);
	const auto& BackBuffers = SwapChain->GetBackBuffer();
	CHECK_RESULT_FALSE(BackBuffers);
	const auto& BackBuffer1 = BackBuffers->at(0);
	CHECK_RESULT_FALSE(BackBuffer1);
	auto Desc = BackBuffer1->GetDesc();

	for (uint32 Count = 0; Count < RTV_BUFFER_NUM; ++Count)
	{
		auto& elem = m_RTVBuffers.at(Count);

		elem = new CRenderTarget();
		CHECK_RESULT_FALSE(elem);

		FRenderTargetInitializer initializer = {};
		
		char Name[128] = {};
		sprintf_s(Name, "%s%d", NameBase, Count);
		initializer.Name = CHash160(Name);

		// 縮小バッファを作成する
		{
			uint32 Divide = Count + 1;
			Desc.Width = Desc.Width / Divide;
			Desc.Height = Desc.Height / Divide;
			initializer.pResDesc = &Desc;
		}

		CHECK_RESULT_FALSE(elem->Initialize(&initializer));
		PRINT("BloomEffect::RenderTargetCreate.Count::%d\n", Count + 1);
	}

	return true;
}

bool CBloomEffectComponent::CreateMeshData()
{
	CResourceManager* Manager = CSubsystemServiceLocator::GetResourceSubsystemEdit()->GetMeshResourceManagerEdit();
	CHECK_RESULT_FALSE(Manager);

	CRenderingSubsystem* Subsystem = CSubsystemServiceLocator::GetRenderingSubsystemEdit();
	CHECK_RESULT_FALSE(Subsystem);
	const auto& Processor = Subsystem->GetProcessor();
	CHECK_RESULT_FALSE(Processor);
	auto MainDevice = Processor->GetDeviceEdit();
	CHECK_RESULT_FALSE(MainDevice);

	FMeshResourceInitializer initializer = {};
	initializer.Name = CPrimitivePlain::GetName();

	const CResource* Res = Manager->SearchResource(initializer.Name);
	if (Res != nullptr)
	{
		m_MeshData = PCast<const CMeshData*>(Res);
		return true;
	}

	initializer.Device = MainDevice;
	initializer.Vertices = CPrimitivePlain::GetVertices();
	initializer.StrideBytes = CPrimitivePlain::GetVerticesStride();
	initializer.VerticesSize = CPrimitivePlain::GetVerticesNum() * CPrimitivePlain::GetVerticesStride();
	initializer.Indices = CPrimitivePlain::GetIndices();
	initializer.IndicesSize = CPrimitivePlain::GetIndicesNum() * sizeof(uint16);

	auto Handle = Manager->SearchCreateResource(&initializer);
	CHECK_RESULT_FALSE(Manager->IsValidHandle(Handle));

	m_MeshData = PCast<const CMeshData*>(Manager->SearchResourceRef(Handle));

	return true;
}

bool CBloomEffectComponent::CreateMaterial(const FBloomPostEffectComponentInitializer* _Initializer)
{
	m_MaterialInterface = new CQuadPolygonMaterial();
	CHECK_RESULT_FALSE(m_MaterialInterface);

	FQuadPolygonMaterialInitializer Initializer = {};
	Initializer.PsoName = _Initializer->QuadPolygonPsoName;

	CHECK_RESULT_FALSE(m_MaterialInterface->Initialize(&Initializer));

	return true;
}
