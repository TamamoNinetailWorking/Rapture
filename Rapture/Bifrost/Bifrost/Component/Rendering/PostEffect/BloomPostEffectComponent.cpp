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

#include <Atlantis/DirectX12/ConstantBuffer/ConstantBufferPreDefine.h>

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

		CHECK_RESULT_FALSE(CreateParameterBuffer());

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

	EDENS_NAMESPACE::Delete(m_ParameterBuffer);

	Super::Finalize();
}

bool CBloomEffectComponent::Draw() const
{
	CRenderingSubsystem* Subsystem = CSubsystemServiceLocator::GetRenderingSubsystemEdit();
	CHECK_RESULT_FALSE(Subsystem);

	CRenderTargetView* RTV = m_RTVBuffers.at(SCast<uint8>(EBloomPostEffectRTVBufferIndex::Bright))->GetRenderTargetView()->GetRenderTargetView();
	CHECK_RESULT_FALSE(RTV);
	Subsystem->SwitchDefaultRenderTargetViewBefore();
	
	{
		// 一括で複数のRTVに書き込む処理は次に対応する

		Subsystem->SwitchRenderTargetViewBefore(RTV);

		{
			Subsystem->SetPrimitiveTopology(Glue::EPrimitiveTopology::TRIANGLELIST);
			Subsystem->SetMeshData(m_MeshData);

			auto& Material = m_MaterialInterface;
			Subsystem->SetMaterialInterface(m_MaterialInterface);

			//uint64 Handle = Subsystem->GetMaterialHeapHandle(Material);
			//auto DescHeap = Material->GetDescriptorHeap();
			//auto Handle = DescHeap->GetGPUDescriptorHandleForHeapStart().ptr;

			//Subsystem->SetGraphicsRootDescriptorTable(1, Handle);

			Subsystem->SetDefaultRenderTargetToSRV(1,0);
			Subsystem->DrawIndexedInstanced(CPrimitivePlain::GetIndicesNum(), 0);
		}

		Subsystem->SwitchRenderTargetViewAfter(RTV);
	}

	Subsystem->SwitchDefaultRenderTargetViewAfter();

	{
		Subsystem->SetPrimitiveTopology(Glue::EPrimitiveTopology::TRIANGLELIST);
		Subsystem->SetMeshData(m_MeshData);

		auto& Material = m_MaterialInterface;
		Subsystem->SetMaterialInterface(m_MaterialInterface);
		
		auto Handle = RTV->GetShaderResourceView()->GetGPUDescriptorHandleForHeapStart();

		Subsystem->SetDescriptorHeap(1, RTV);
		Subsystem->SetGraphicsRootDescriptorTable(0, Handle.ptr);
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

	for (uint32 Count = 0; Count < SCast<uint8>(EBloomPostEffectRTVBufferIndex::RTV_BUFFER_NUM); ++Count)
	{
		auto& elem = m_RTVBuffers.at(Count);

		elem = new CRenderTarget();
		CHECK_RESULT_FALSE(elem);

		FRenderTargetInitializer initializer = {};
		
		char Name[128] = {};
		sprintf_s(Name, "%s%d", NameBase, Count);
		initializer.Name = CHash160(Name);

		// 縮小バッファを作成する
		if(
			(Count != SCast<uint8>(EBloomPostEffectRTVBufferIndex::Result)) 
			&& (Count != SCast<uint8>(EBloomPostEffectRTVBufferIndex::Bright))
			)
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
	CHECK_RESULT_FALSE(_Initializer);

	{
		m_MaterialInterface = new CQuadPolygonMaterial();
		CHECK_RESULT_FALSE(m_MaterialInterface);

		FQuadPolygonMaterialInitializer Initializer = {};
		Initializer.PsoName = _Initializer->QuadPolygonPsoName;

		CHECK_RESULT_FALSE(m_MaterialInterface->Initialize(&Initializer));
	}

	{
		m_BrightnessInterface = new CQuadPolygonMaterial();
		CHECK_RESULT_FALSE(m_BrightnessInterface);

		FQuadPolygonMaterialInitializer Initializer = {};
		Initializer.PsoName = _Initializer->BrightnessPsoName;

		CHECK_RESULT_FALSE(m_BrightnessInterface->Initialize(&Initializer));
	}

	{
		m_BlurInterface = new CQuadPolygonMaterial();
		CHECK_RESULT_FALSE(m_BlurInterface);

		FQuadPolygonMaterialInitializer Initializer = {};
		Initializer.PsoName = _Initializer->BlurPsoName;

		CHECK_RESULT_FALSE(m_BlurInterface->Initialize(&Initializer));
	}

	{
		m_FetchColorInterface = new CQuadPolygonMaterial();
		CHECK_RESULT_FALSE(m_FetchColorInterface);

		FQuadPolygonMaterialInitializer Initializer = {};
		Initializer.PsoName = _Initializer->FetchColorPsoName;

		CHECK_RESULT_FALSE(m_FetchColorInterface->Initialize(&Initializer));
	}
	return true;
}

bool CBloomEffectComponent::CreateParameterBuffer()
{
	m_ParameterBuffer = new FBloomPostEffectParameterBuffer();
	CHECK_RESULT_FALSE(m_ParameterBuffer);

	D3D12_CONSTANT_BUFFER_VIEW_DESC Desc = {};
	uint32 bufferSize = AlignBufferSize(sizeof(FBloomPostEffectParameterBuffer));
	Desc.SizeInBytes = bufferSize;

	//D3D12_CPU_DESCRIPTOR_HANDLE handle = m_BlurInterface->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	
	//----------------
	// 途中
	//----------------

	return true;
}

const FBloomPostEffectParameterBuffer CBloomEffectComponent::CalcBlurParam(uint32 _Width, uint32 _Height, const ATLANTIS_NAMESPACE::Glue::Vector2& _Direction, float _Deviation)
{
	auto GaussianDistribution = [](const Glue::Vector2& _Base, float _Rho)
	{
		return expf(-(_Base.x * _Base.x + _Base.y * _Base.y) / (2.0f * _Rho * _Rho));
	};

	FBloomPostEffectParameterBuffer Result = {};
	Result.SampleCount = 15;

	auto tu = 1.f / _Width;
	auto tv = 1.f / _Height;

	{
		Result.Offset[0].x = 0.f;
		Result.Offset[0].y = 0.f;
		Result.Offset[0].z = GaussianDistribution(Glue::Vector2(0.f, 0.f), _Deviation);
	}

	auto TotalWeight = Result.Offset[0].z;

	for (uint32 Count = 1; Count / 8; ++Count)
	{
		Result.Offset[Count].x = _Direction.x * Count * tu;
		Result.Offset[Count].y = _Direction.y * Count * tv;

		Glue::Vector2 MultiDir(_Direction.x * Count, _Direction.y * Count);
		Result.Offset[Count].z = GaussianDistribution(MultiDir, _Deviation);
		
		TotalWeight += Result.Offset[Count].z * 2.f;
	}

	for (uint32 Count = 0; Count < 8; ++Count)
	{
		Result.Offset[Count].z /= TotalWeight;
	}

	for (uint32 Count = 8; Count < 15; ++Count)
	{
		Result.Offset[Count].x = -Result.Offset[Count - 7].x;
		Result.Offset[Count].y = -Result.Offset[Count - 7].y;
		Result.Offset[Count].z = -Result.Offset[Count - 7].z;
	}

	return Result;

}
