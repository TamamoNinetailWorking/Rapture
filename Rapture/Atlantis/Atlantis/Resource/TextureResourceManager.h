#pragma once

#include <eden/include/Resource/ResourceManager.h>

EDENS_NAMESPACE_BEGIN

class CResource;

EDENS_NAMESPACE_END

ATLANTIS_NAMESPACE_BEGIN

class CTextureResourceManager : public EDENS_NAMESPACE::CResourceManager
{
protected:

	EDENS_NAMESPACE::CResource* CreateResourceObject() override;

};

ATLANTIS_NAMESPACE_END