#pragma once

#include <eden/include/Resource/ResourceManager.h>

EDENS_NAMESPACE_BEGIN

struct FResourceInitializerBase;

EDENS_NAMESPACE_END

ATLANTIS_NAMESPACE_BEGIN

class CShaderManager : public EDENS_NAMESPACE::CResourceManager
{
protected:

	EDENS_NAMESPACE::FResourceHandle CreateResourceImpl(const EDENS_NAMESPACE::FResourceInitializerBase* _Initializer) override;

};

ATLANTIS_NAMESPACE_END