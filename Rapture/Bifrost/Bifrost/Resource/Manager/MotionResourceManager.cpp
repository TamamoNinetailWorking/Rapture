#include "MotionResourceManager.h"

#include <Bifrost/Resource/Motion/VmdMotionResourceInitializer.h>
#include <Bifrost/Resource/Motion/VmdMotionResource.h>

USING_BIFROST;
EDENS_NAMESPACE_USING;

FResourceHandle CMotionResourceManager::CreateResourceImpl(const FResourceInitializerBase* _Initializer)
{
	const FMotionResourceInitializerBase* baseInit = PCast<const FMotionResourceInitializerBase*>(_Initializer);
	if (_Initializer == nullptr) { return GetInvalidHandle(); }

	switch (baseInit->Type)
	{
	case EMotionType::MOTION_TYPE_VMD:
		return CreateResource<CVmdMotionResource>(_Initializer);

	default:
		return GetInvalidHandle();
	}
}