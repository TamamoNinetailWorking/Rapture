#pragma once

#include <Bifrost/Component/ComponentManager.h>
#include "LightComponent.h"
#include "LightComponentHandle.h"


BIFROST_NAMESPACE_BEGIN

class CLightComponentManager : public TComponentManager<CLightComponent>
{
public:

	using Handle = FLightComponentManagerHandle;

};

BIFROST_NAMESPACE_END