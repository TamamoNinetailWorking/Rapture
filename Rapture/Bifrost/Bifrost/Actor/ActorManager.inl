#pragma once

#include "Actor.h"

BIFROST_NAMESPACE_BEGIN

template <class Actor>
CActor* CActorManager::CreateActor(const FActorInitializerBase* _Initializer)
{
#define CHECK_RETURN_NULL(param) \
    if(!param) { return nullptr; } \

    //CHECK_RETURN_NULL(_Initializer);

    Actor* actor = new Actor();
    CHECK_RETURN_NULL(actor);

    if (!actor->Initialize(_Initializer))
    {
        delete actor;
        actor = nullptr;
        return nullptr;
    }

    m_ActorList->push_back(actor);

#undef CHECK_RETURN_NULL

    return actor;
}


BIFROST_NAMESPACE_END