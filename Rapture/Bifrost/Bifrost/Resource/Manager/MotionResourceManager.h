#pragma once

#include <eden/include/Resource/ResourceManager.h>

BIFROST_NAMESPACE_BEGIN

class CMotionResourceManager : public EDENS_NAMESPACE::CResourceManager
{
protected:

	EDENS_NAMESPACE::FResourceHandle CreateResourceImpl(const EDENS_NAMESPACE::FResourceInitializerBase* _Initializer) override;

};


BIFROST_NAMESPACE_END