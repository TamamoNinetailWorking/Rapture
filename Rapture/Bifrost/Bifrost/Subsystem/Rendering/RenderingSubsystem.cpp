#include "RenderingSubsystem.h"
#include "RenderingSubsystemInitializer.h"

#include <eden/include/utility/ender_utility.h>

#include <Atlantis/RHIProccessor/RHIProcessor.h>
#include <Atlantis/RHIProccessor/RHIProcessorInitialize.h>
#include <Atlantis/RHIProccessor/RHIRenderTargetView.h>
#include <Atlantis/RHIProccessor/RHIRenderTargetViewInitializer.h>
#include <Atlantis/SceneView/SceneView.h>
#include <Atlantis/SceneView/SceneViewInitializer.h>
#include <Bifrost/Subsystem/Rendering/RenderingQueue/RenderingQueue.h>
#include <Bifrost/Subsystem/Rendering/RenderingQueue/RenderingQueueInitializer.h>

#include <Atlantis/DirectX12/Command/CommandContext.h>
#include <Atlantis/DirectX12/MainDevice/MainDevice.h>

#include <Bifrost/Component/Rendering/RenderingComponent.h>
#include <Atlantis/Material/MaterialInterface.h>
#include <Bifrost/Model/MeshData/MeshData.h>
#include <Atlantis/DirectX12/VertexBuffer/VertexBuffer.h>
#include <Atlantis/DirectX12/IndexBuffer/IndexBuffer.h>
#include <Atlantis/DirectX12/RenderTargetView/RenderTargetView.h>
#include <Atlantis/DirectX12/DepthStencilView/DepthStencilView.h>

USING_BIFROST;
USING_ATLANTIS;

CRenderingSubsystem::CRenderingSubsystem()
{
}

CRenderingSubsystem::~CRenderingSubsystem()
{
}

bool CRenderingSubsystem::Initialize(const FSubsystemInitializerBase* _Initializer)
{
	do
	{
		CHECK_RESULT_BREAK(_Initializer);
		const FRenderingSubsystemInitializer* initializer = PCast<const FRenderingSubsystemInitializer*>(_Initializer);
		CHECK_RESULT_BREAK(initializer);

		CHECK_RESULT_BREAK(CreateProcessor(initializer));
		CHECK_RESULT_BREAK(CreateRenderingQueue(initializer));
		CHECK_RESULT_BREAK(CreateRTV(initializer));
		CHECK_RESULT_BREAK(CreateScreenView(initializer));

		return true;
	} while (0);

	Finalize();
	return false;
}

void CRenderingSubsystem::Finalize()
{
	EDENS_NAMESPACE::FinalizeObject(m_SceneView);
	EDENS_NAMESPACE::FinalizeObject(m_RTV);

	for (auto& elem : m_RenderGroup)
	{
		if (!elem) { continue; }
		EDENS_NAMESPACE::FinalizeObject(elem);
	}

	EDENS_NAMESPACE::FinalizeObject(m_Processor);
}

void CRenderingSubsystem::SetRenderObject(ERenderGroup _Group, const CRenderingComponent* _Component)
{
	if (_Group >= ERenderGroup::RENDER_GROUP_INVALID)
	{
		PRINT("CRenderingSubsystem::SetRenderObject INVALID GROUP.\n");
		return;
	}
	RenderingQueue* queue = m_RenderGroup.at(UNumCast(_Group));
	CHECK(queue);
	queue->SetRenderComponent(_Component);
}

void CRenderingSubsystem::ClearRenderGroup()
{
	for (auto& elem : m_RenderGroup)
	{
		elem->Clear();
	}
}

bool CRenderingSubsystem::RenderBegin()
{
	return m_Processor->RenderBegin(m_RTV, m_SceneView);
}

void CRenderingSubsystem::Rendering()
{
	// RenderBegin的な感じでProcessor達の処理をまとめる
	// >> ExecuteするのはProcessorのイメージ
	//CHECK(m_Processor->RenderBegin(m_RTV, m_SceneView));

	for (auto& elem : m_RenderGroup)
	{
		// RenderComponentをTestコードで作成して、Subsystemをテスト
		// 描画できればModelComponentを作成する
		RenderQueue(elem);
	}

	//m_Processor->RenderEnd(m_RTV);

	//// 登録される処理順的にここ
	//ClearRenderGroup();
}

void CRenderingSubsystem::RenderEnd()
{
	m_Processor->RenderEnd(m_RTV);
	
	// 登録される処理順的にここ
	ClearRenderGroup();
}

const CRenderingSubsystem::Processor* CRenderingSubsystem::GetProcessor() const
{
	return m_Processor;
}

CRenderingSubsystem::Processor* CRenderingSubsystem::GetProcessorEdit() const
{
	return m_Processor;
}

const CRenderingSubsystem::RTVStruct* CRenderingSubsystem::GetRTV() const
{
	return m_RTV;
}

CRenderingSubsystem::RTVStruct* CRenderingSubsystem::GetRTVEdit() const
{
	return m_RTV;
}

const CRenderingSubsystem::SceneView* CRenderingSubsystem::GetSceneView() const
{
	return m_SceneView;
}

CRenderingSubsystem::SceneView* CRenderingSubsystem::GetSceneViewEdit() const
{
	return m_SceneView;
}

void CRenderingSubsystem::SortQueue()
{
	// ソート処理が必要になったら入れる
}

bool CRenderingSubsystem::SetPrimitiveTopology(Glue::EPrimitiveTopology _Topology)
{
	return m_Processor->SetPrimitiveTopology(_Topology);
}

bool CRenderingSubsystem::SetMeshData(const CMeshData* _Mesh)
{
	CHECK_RESULT_FALSE(_Mesh);
	m_Processor->SetVertexBuffer(_Mesh->GetVertexBuffer());
	m_Processor->SetIndexBuffer(_Mesh->GetIndexBuffer());

	return true;
}

bool CRenderingSubsystem::SetMaterialInterface(const IMaterialInterface* _Material)
{
	CHECK_RESULT_FALSE(_Material);
	m_Processor->SetGraphicsPipeline(_Material);

	return true;
}

bool CRenderingSubsystem::SetDescriptorHeap(const IMaterialInterface* _Material)
{
	CHECK_RESULT_FALSE(_Material);

	auto bufDescHeap = _Material->GetDescriptorHeap();
	auto bufHeapHandle = bufDescHeap->GetGPUDescriptorHandleForHeapStart();

	CHECK_RESULT_FALSE(m_Processor->SetDescriptorHeaps(1, bufDescHeap));
	
	return true;
}

bool CRenderingSubsystem::SetGraphicsRootDescriptorTable(uint32 _Offset, uint64 _HeapHandle)
{
	return m_Processor->SetGraphicsRootDescriptorTable(_Offset, _HeapHandle);
}

uint64 CRenderingSubsystem::GetMaterialHeapHandle(const IMaterialInterface* _Material)
{
	if (!_Material) { return 0; };
	
	auto matHeapHandle = _Material->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();

	return matHeapHandle.ptr;
}

uint64 CRenderingSubsystem::IncrementHeapHandle(uint64 _Handle) const
{
	return 	_Handle + m_Processor->GetDescriptorHandleIncrementSize(Glue::EDescriptorHeapType::CBV_SRV_UAV);
}

bool CRenderingSubsystem::DrawIndexedInstanced(uint32 _CurrentIndex, uint32 _IndexOffset)
{
	return m_Processor->DrawIndexedInstanced(_CurrentIndex, _IndexOffset);
}

bool CRenderingSubsystem::SwitchRenderTargetViewBefore(ATLANTIS_NAMESPACE::CRenderTargetView* _RTV)
{
	CHECK_RESULT_FALSE(_RTV);
	CHECK_RESULT_FALSE(_RTV->GetDescriptorHeap());
	auto Handle = _RTV->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();

	CHECK_RESULT_FALSE(m_RTV->GetDepthStencilView());
	auto DSV = m_RTV->GetDepthStencilView()->GetDescriptorHeap();
	CHECK_RESULT_FALSE(DSV);
	auto DSVHandle = DSV->GetCPUDescriptorHandleForHeapStart();

	auto Context = m_Processor->GetContextEdit();
	CHECK_RESULT_FALSE(Context);
	auto CmdList = Context->GetCommandList();
	CHECK_RESULT_FALSE(CmdList);

	CHECK_RESULT_FALSE(m_Processor->ChangeRenderTargetBarrierBefore(_RTV));

	CmdList->OMSetRenderTargets(1, &Handle, false, &DSVHandle);
	Context->ClearRenderTargetView(_RTV, RenderTargetClearColor);

	return true;
}

bool CRenderingSubsystem::SwitchRenderTargetViewAfter(const ATLANTIS_NAMESPACE::CRenderTargetView* _RTV)
{
	CHECK_RESULT_FALSE(m_Processor->ChangeRenderTargetBarrierAfter(_RTV));

	return true;
}

bool CRenderingSubsystem::SetDefaultRenderTargetToSRV(uint32 _Index)
{
	auto Context = m_Processor->GetContextEdit();
	CHECK_RESULT_FALSE(Context);
	auto CmdList = Context->GetCommandList();
	CHECK_RESULT_FALSE(CmdList);

	auto Handle = m_RTV->GetCurrentRenderTargetView()->GetShaderResourceView()->GetGPUDescriptorHandleForHeapStart();

	CHECK_RESULT_FALSE(SetGraphicsRootDescriptorTable(_Index, Handle.ptr));

	return true;
}

bool CRenderingSubsystem::SwitchDefaultRenderTargetViewBefore()
{
	CHECK_RESULT_FALSE(m_Processor->ChangeRenderTargetBarrier(m_RTV->GetCurrentRenderTargetView(), Glue::EResourceState::RESOURCE_STATE_RENDER_TARGET, Glue::EResourceState::RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

	return true;
}

bool CRenderingSubsystem::SwitchDefaultRenderTargetViewAfter()
{
	CHECK_RESULT_FALSE(m_RTV->GetCurrentRenderTargetView());
	auto Handle = m_RTV->GetCurrentRenderTargetView()->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();

	CHECK_RESULT_FALSE(m_RTV->GetDepthStencilView());
	auto DSV = m_RTV->GetDepthStencilView()->GetDescriptorHeap();
	CHECK_RESULT_FALSE(DSV);
	auto DSVHandle = DSV->GetCPUDescriptorHandleForHeapStart();

	auto Context = m_Processor->GetContextEdit();
	CHECK_RESULT_FALSE(Context);
	auto CmdList = Context->GetCommandList();
	CHECK_RESULT_FALSE(CmdList);

	CHECK_RESULT_FALSE(m_Processor->ChangeRenderTargetBarrier(m_RTV->GetCurrentRenderTargetView(), Glue::EResourceState::RESOURCE_STATE_PIXEL_SHADER_RESOURCE, Glue::EResourceState::RESOURCE_STATE_RENDER_TARGET));

	CmdList->OMSetRenderTargets(1, &Handle, false, &DSVHandle);

	return true;
}

void CRenderingSubsystem::RenderQueue(const RenderingQueue* _Queue)
{
	CHECK(_Queue);

	CHECK(m_Processor);

	auto Context = m_Processor->GetContextEdit();
	CHECK(Context);

	auto CmdList = Context->GetCommandList();
	CHECK(CmdList);

	for (uint32 index = 0; index < _Queue->GetQueueNum(); ++index)
	{
		auto elem = _Queue->GetRenderData(index);

		elem->Draw();

#if 0
		// ここはせめてCmdListを表に出さない形に直さないといけないだろう
		// >>CommandContextの処理の方を
		CmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		auto meshData = elem->GetMeshData();
		CHECK(meshData);
		CHECK(meshData->GetVertexBuffer());
		CHECK(meshData->GetIndexBuffer());

		Context->SetVertexBuffer(meshData->GetVertexBuffer());
		Context->SetIndexBuffer(meshData->GetIndexBuffer());

		auto material = elem->GetMaterialInterface();
		CHECK(material);

		Context->SetPipelineState(material->GetGraphicsPipeline());
		Context->SetRootSignature(material->GetRootSignature());


		auto descHeap = material->GetDescriptorHeap();

		auto bufDescHeap = descHeap;
		auto bufHeapHandle = bufDescHeap->GetGPUDescriptorHandleForHeapStart();
		CmdList->SetDescriptorHeaps(1, &bufDescHeap);
		CmdList->SetGraphicsRootDescriptorTable(0, bufHeapHandle);
		
		auto heapStride = material->GetHeapStride();
		
		// MaterialBuffer
		auto matHeapHandle = bufHeapHandle;
		matHeapHandle.ptr += m_Processor->GetDevice()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		uint32 indexOffset = 0;
		for (uint32 count = 0; count < material->GetMaterialNum(); ++count)
		{
			uint32 currentIndex = material->GetDrawIndex(count);

			CmdList->SetGraphicsRootDescriptorTable(1, matHeapHandle);
			CmdList->DrawIndexedInstanced(currentIndex, 1, indexOffset, 0, 0);

			indexOffset += currentIndex;
			matHeapHandle.ptr += heapStride;
		}
#endif
#if 0
		SetPrimitiveTopology(Glue::EPrimitiveTopology::TRIANGLELIST);

		SetMeshData(elem->GetMeshData());

		auto material = elem->GetMaterialInterface();

		SetMaterialInterface(elem->GetMaterialInterface());

		auto heapStride = material->GetHeapStride();

		// MaterialBuffer
		uint64 handle = GetMaterialHeapHandle(material);

		uint32 indexOffset = 0;
		for (uint32 count = 0; count < material->GetMaterialNum(); ++count)
		{
			uint32 currentIndex = material->GetDrawIndex(count);

			SetGraphicsRootDescriptorTable(1, handle);
			DrawIndexedInstanced(currentIndex, indexOffset);

			indexOffset += currentIndex;
			handle += heapStride;
		}
#endif
	}
}

bool CRenderingSubsystem::CreateProcessor(const FRenderingSubsystemInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	m_Processor = new Processor();
	CHECK_RESULT_FALSE(m_Processor);

	FRHIProcessorInitialize initializer = {};
	initializer.BarrierType = _Initializer->BarrierType;
	initializer.CommandListType = _Initializer->CommandListType;
	initializer.CommandQueueFlag = _Initializer->CommandQueueFlag;
	initializer.CommandQueuePriority = _Initializer->CommandQueuePriority;

	return m_Processor->Initialize(&initializer);
}

bool CRenderingSubsystem::CreateRenderingQueue(const FRenderingSubsystemInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	FRenderingQueueInitializer initializer = {};
	initializer.m_QueueReserveNum = _Initializer->QueueReserveNum;

	for (auto& elem : m_RenderGroup)
	{
		elem = new RenderingQueue();
		CHECK_RESULT_FALSE(elem);

		CHECK_RESULT_FALSE(elem->Initialize(&initializer));
	}

	return true;
}

bool CRenderingSubsystem::CreateRTV(const FRenderingSubsystemInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(m_Processor);

	m_RTV = new RTVStruct();
	CHECK_RESULT_FALSE(m_RTV);

	FRHIRenderTargetInitializer initializer = {};
	initializer.Device = m_Processor->GetDevice();
	initializer.CommandQueue = m_Processor->GetQueue();
	initializer.Format = _Initializer->RTVFormat;
	initializer.RTVDimension = _Initializer->RTVDimension;
	initializer.ViewWidth = _Initializer->ViewWidth;
	initializer.ViewHeight = _Initializer->ViewHeight;
	initializer.WindowHandle = _Initializer->WindowHandle;

	return m_RTV->Initialize(&initializer);
}

bool CRenderingSubsystem::CreateScreenView(const FRenderingSubsystemInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	m_SceneView = new SceneView();
	CHECK_RESULT_FALSE(m_SceneView);

	FSceneViewInitializer initializer = {};
	initializer.ViewportWidth = SCast<float>(_Initializer->ViewWidth);
	initializer.ViewportHeight = SCast<float>(_Initializer->ViewHeight);
	initializer.FovAngle = _Initializer->FovAngle;
	initializer.NearZ = _Initializer->NearZ;
	initializer.FarZ = _Initializer->FarZ;

	return m_SceneView->Initialize(&initializer);
}
