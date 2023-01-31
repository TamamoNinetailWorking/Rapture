#include <Bifrost/Component/ComponentManager.h>

#include <eden/include/utility/ender_utility.h>

#include <Bifrost/Component/Component.h>

USING_BIFROST;

CComponentManager::CComponentManager()
{
}

CComponentManager::~CComponentManager()
{
}

bool CComponentManager::Initialize()
{
    do
    {
        m_ComponentList = new ComponentList();
        CHECK_RESULT_BREAK(m_ComponentList);



        return true;
    } while (0);

    Finalize();
    return false;
}

void CComponentManager::Finalize()
{
    ListFinalize();
    Delete(m_ComponentList);
}

void CComponentManager::Update(float _DeltaTime)
{
    CHECK(m_ComponentList);

    for (auto& elem : *m_ComponentList)
    {
        if (elem == nullptr) { continue; }
        elem->Update(_DeltaTime);
    }

}

void CComponentManager::ListFinalize()
{
    CHECK(m_ComponentList);

    for (auto& elem : *m_ComponentList)
    {
        if (!elem) { continue; }
        elem->Finalize();
        elem = nullptr;
    }

    m_ComponentList->clear();
}
