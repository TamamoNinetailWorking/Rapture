#include <Bifrost/Actor/ActorManager.h>

#include <eden/include/utility/ender_utility.h>

#include <Bifrost/Actor/Actor.h>



USING_BIFROST;

CActorManager::CActorManager()
{
}

CActorManager::~CActorManager()
{
}

bool CActorManager::Initialize()
{
    do
    {
        m_ActorList = new ActorList();
        CHECK_RESULT_BREAK(m_ActorList);



        return true;
    } while (0);

    Finalize();
    return false;
}

void CActorManager::Finalize()
{
    ListFinalize();
    Delete(m_ActorList);
}

//void CActorManager::Update(float _DeltaTime)
//{
//    CHECK(m_ActorList);
//
//    for (auto& elem : *m_ActorList)
//    {
//        if (elem == nullptr) { continue; }
//        elem->Update(_DeltaTime);
//    }
//
//    return;
//}

void CActorManager::ListFinalize()
{
    CHECK(m_ActorList);

    for (auto& elem : *m_ActorList)
    {
        if (!elem) { continue; }
        elem->Finalize();
        elem = nullptr;
    }

    m_ActorList->clear();
}
