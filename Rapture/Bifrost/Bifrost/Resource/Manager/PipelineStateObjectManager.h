#pragma once

#include <eden/include/Resource/ResourceManager.h>

EDENS_NAMESPACE_BEGIN

struct FResourceInitializerBase;

EDENS_NAMESPACE_END


BIFROST_NAMESPACE_BEGIN

class CPipelineStateObjectManager : public EDENS_NAMESPACE::CResourceManager
{
protected:

	EDENS_NAMESPACE::FResourceHandle CreateResourceImpl(const EDENS_NAMESPACE::FResourceInitializerBase* _Initializer) override;

};

BIFROST_NAMESPACE_END