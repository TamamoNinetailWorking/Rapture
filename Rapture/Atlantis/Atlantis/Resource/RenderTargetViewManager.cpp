#include "RenderTargetViewManager.h"

#include <Atlantis/DirectX12/RenderTargetView/RenderTargetViewResource.h>

USING_ATLANTIS;

EDENS_NAMESPACE::FResourceHandle ATLANTIS_NAMESPACE::CRenderTargetViewManager::CreateResourceImpl(const EDENS_NAMESPACE::FResourceInitializerBase* _Initializer)
{
	if (_Initializer == nullptr) { return GetInvalidHandle(); }

	return CreateResource<CRenderTargetViewResource>(_Initializer);
}
