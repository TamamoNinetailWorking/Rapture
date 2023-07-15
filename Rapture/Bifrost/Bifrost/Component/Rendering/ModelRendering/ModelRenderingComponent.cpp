#include "ModelRenderingComponent.h"

#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Rendering/RenderingSubsystem.h>

#include <Atlantis/Material/MaterialInterface.h>

USING_BIFROST;

CModelRenderingComponent::CModelRenderingComponent() : Super()
{
	m_RenderGroup = ERenderGroup::RENDER_GROUP_DEFAULT;
}

bool CModelRenderingComponent::Draw() const
{
	CHECK_RESULT_FALSE(Super::Draw());

	CRenderingSubsystem* Subsystem = CSubsystemServiceLocator::GetRenderingSubsystemEdit();
	CHECK_RESULT_FALSE(Subsystem);

	Subsystem->SetMeshData(m_MeshData);

	auto& Material = m_MaterialInterface;
	Subsystem->SetMaterialInterface(m_MaterialInterface);
	Subsystem->SetDescriptorHeap(m_MaterialInterface);

	uint64 Handle = Subsystem->GetMaterialHeapHandle(Material);
	Subsystem->SetGraphicsRootDescriptorTable(0, Handle);
	Handle = Subsystem->IncrementHeapHandle(Handle);

	uint64 HeapStride = Material->GetHeapStride();

	for (uint32 Count = 0, IndexOffset = 0; Count < Material->GetMaterialNum(); ++Count)
	{
		uint32 CurrentIndex = Material->GetDrawIndex(Count);

		Subsystem->SetGraphicsRootDescriptorTable(1, Handle);
		Subsystem->DrawIndexedInstanced(CurrentIndex, IndexOffset);

		IndexOffset += CurrentIndex;
		Handle += HeapStride;
	}

	return true;
}
