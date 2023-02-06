#pragma once

#include <Bifrost/Actor/ActorManager.h>

BIFROST_NAMESPACE_BEGIN

template<class Actor>
inline CActor* CActorSubsystem::CreateActor(const FActorInitializerBase* _Initializer)
{
	return m_ActorManager->CreateActor<Actor>(_Initializer);
}


BIFROST_NAMESPACE_END