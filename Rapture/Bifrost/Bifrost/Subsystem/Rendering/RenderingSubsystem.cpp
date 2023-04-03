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

#include <Bifrost/Component/RenderingComponent.h>
#include <Atlantis/Material/MaterialInterface.h>
#include <Bifrost/Model/MeshData/MeshData.h>
#include <Atlantis/DirectX12/VertexBuffer/VertexBuffer.h>
#include <Atlantis/DirectX12/IndexBuffer/IndexBuffer.h>

USING_BIFROST;
USING_ATLANTIS;

CRenderingSubsystem::CRenderingSubsystem()
{
}

CRenderingSubsystem::~CRenderingSubsystem()
{
}

bool CRenderingSubsystem::Initialize(const FRenderingSubsystemInitializer* _Initializer)
{
	do
	{
		CHECK_RESULT_BREAK(_Initializer);

		CHECK_RESULT_BREAK(CreateProcessor(_Initializer));
		CHECK_RESULT_BREAK(CreateRenderingQueue(_Initializer));
		CHECK_RESULT_BREAK(CreateRTV(_Initializer));
		CHECK_RESULT_BREAK(CreateScreenView(_Initializer));

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

void CRenderingSubsystem::Rendering()
{
	// RenderBegin的な感じでProcessor達の処理をまとめる
	// >> ExecuteするのはProcessorのイメージ
	CHECK(m_Processor->RenderBegin(m_RTV, m_SceneView));

	for (auto& elem : m_RenderGroup)
	{
		// RenderComponentをTestコードで作成して、Subsystemをテスト
		// 描画できればModelComponentを作成する
		RenderQueue(elem);
	}

	m_Processor->RenderEnd(m_RTV);
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

void CRenderingSubsystem::RenderBegin()
{
	CHECK(m_Processor->RenderBegin(m_RTV, m_SceneView));
}

void CRenderingSubsystem::RenderEnd()
{
	m_Processor->RenderEnd(m_RTV);
}
