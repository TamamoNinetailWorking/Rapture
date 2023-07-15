#include "TextureResourceManager.h"

#include <Atlantis/DirectX12/Texture/TextureResource.h>


USING_ATLANTIS;
EDENS_NAMESPACE_USING;

FResourceHandle CTextureResourceManager::CreateResourceImpl(const FResourceInitializerBase* _Initializer)
{
	if (_Initializer == nullptr) { return GetInvalidHandle(); }

	return CreateResource<CTextureResource>(_Initializer);
}
