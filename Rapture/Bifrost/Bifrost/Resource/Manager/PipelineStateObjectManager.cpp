#include "PipelineStateObjectManager.h"

#include <Bifrost/Resource/PSO/PipelineStateObject.h>

USING_BIFROST;
EDENS_NAMESPACE_USING;

FResourceHandle CPipelineStateObjectManager::CreateResourceImpl(const FResourceInitializerBase* _Initializer)
{
	if (_Initializer == nullptr) { return GetInvalidHandle(); }

	return CreateResource<CPipelineStateObject>(_Initializer);
}