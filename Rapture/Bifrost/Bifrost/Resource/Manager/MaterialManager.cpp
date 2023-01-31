#include "MaterialManager.h"
#include <Bifrost/Resource/Material/MaterialInitializerBase.h>

#include <Bifrost/Model/Pmd/PmdMaterialResource.h>


USING_BIFROST;
EDENS_NAMESPACE_USING;

FResourceHandle CMaterialManager::CreateResourceImpl(const FResourceInitializerBase* _Initializer)
{
	const FMaterialInitializerBase* baseInit = PCast<const FMaterialInitializerBase*>(_Initializer);
	if (_Initializer == nullptr) { return GetInvalidHandle(); }

	switch (baseInit->Type)
	{
	case EMaterialType::MATERIAL_TYPE_PMD:
		return CreateResource<CPmdMaterialResource>(_Initializer);

	default:
		return GetInvalidHandle();
	}
}
