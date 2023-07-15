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

CActor* CActorManager::SearchActor(const Hash160& _Name)
{
    for (auto& actor : *m_ActorList)
    {
        if (actor->GetHash() == _Name)
        {
            return actor;
        }
    }

    return nullptr;
}


void CActorManager::DeleteActor(const Hash160& _Name)
{
    for (auto itr = m_ActorList->begin(); itr != m_ActorList->end(); ++itr)
    {
        auto elem = *itr;
        if (!elem) { continue; }
        if (elem->GetHash() != _Name) { continue; }

        elem->SetPendingKill(true);
        elem->ReserveKill();
        //itr = m_ActorList->erase(itr);
        return;
    }
}

void CActorManager::DeleteActor(const CActor* _Actor)
{
    DeleteActor(_Actor->GetHash());
}

void CActorManager::DestroyActors()
{
    for (auto itr = m_ActorList->begin(); itr != m_ActorList->end(); ++itr)
    {
        auto elem = *itr;
        if (!elem) { continue; }
        if (!elem->IsPendingKill()) { continue; }

        itr = m_ActorList->erase(itr);
    }
}

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
