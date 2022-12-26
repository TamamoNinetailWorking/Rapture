#include "TextureResourceManager.h"

#include <Atlantis/DirectX12/Texture/TextureResource.h>


USING_ATLANTIS;
EDENS_NAMESPACE_USING;

CResource* CTextureResourceManager::CreateResourceObject()
{
	CTextureResource* ptr = new CTextureResource();
	return ptr;
}
