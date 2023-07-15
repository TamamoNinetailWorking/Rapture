#include "MeshDataManager.h"

#include <Bifrost/Model/MeshData/MeshDataInitializer.h>
#include <Bifrost/Model/MeshData/MeshData.h>

USING_BIFROST;
EDENS_NAMESPACE_USING;

FResourceHandle CMeshDataManager::CreateResourceImpl(const FResourceInitializerBase* _Initializer)
{
	const FMeshResourceInitializer* initializer = PCast<const FMeshResourceInitializer*>(_Initializer);

	return CreateResource<CMeshData>(initializer);
}
