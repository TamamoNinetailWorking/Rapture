#pragma once

#include <Bifrost/Component/ComponentManager.h>
#include "CameraComponent.h"
#include "CameraComponentHandle.h"


BIFROST_NAMESPACE_BEGIN

class CCameraComponentManager : public TComponentManager<CCameraComponent>
{
public:

	using Handle = FCameraComponentManagerHandle;

};

BIFROST_NAMESPACE_END