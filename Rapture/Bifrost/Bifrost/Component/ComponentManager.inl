#pragma once

#include <eden/include/utility/ender_utility.h>
#include "ComponentManager.h"

BIFROST_NAMESPACE_BEGIN

template <class TComponent>
bool TComponentManager<TComponent>::Initialize()
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

template <class TComponent>
void TComponentManager<TComponent>::Finalize()
{
    ListFinalize();
    Delete(m_ComponentList);
}

template<class TComponent>
inline typename FComponentManagerHandle<TComponent> 
TComponentManager<TComponent>::RegistComponent
(
    TComponent* _Component
)
{
    if (!_Component) { return TCMHandle(); }

    m_ComponentList->push_back(_Component);

    TCMHandle handle = {};
    handle.Handle = --m_ComponentList->end();
    handle.IsActive = true;

    PRINT("ComponentManager Regist.\n");

    return handle;
}

template<class TComponent>
inline void TComponentManager<TComponent>::DeleteComponent(typename FComponentManagerHandle<TComponent>& _Handle)
{
    if (IsHandleInvalid(_Handle)) { return; }
    m_ComponentList->erase(_Handle.Handle);
    _Handle.IsActive = false;
}

template<class TComponent>
inline const TComponent* TComponentManager<TComponent>::SearchComponent(const typename FComponentManagerHandle<TComponent>& _Handle)
{
    if (IsHandleInvalid(_Handle)) { return nullptr; }
    const TComponent* comp = *(_Handle.Handle);
    return comp;
}

template<class TComponent>
inline TComponent* TComponentManager<TComponent>::SearchComponentEdit(const typename FComponentManagerHandle<TComponent>& _Handle)
{
    if (IsHandleInvalid(_Handle)) { return nullptr; }
    TComponent* comp = *(_Handle.Handle);
    return comp;
}

template <class TComponent>
void TComponentManager<TComponent>::ListFinalize()
{
    CHECK(m_ComponentList);

    for (auto& elem : *m_ComponentList)
    {
        if (!elem) { continue; }
        elem = nullptr;
    }

    m_ComponentList->clear();
}

template<class TComponent>
inline bool 
TComponentManager<TComponent>::IsHandleInvalid
(
    const typename FComponentManagerHandle<TComponent>& _Handle
)
{
    if (_Handle.IsActive == false) { return true; }
    return _Handle.Handle == m_ComponentList->end();
}

BIFROST_NAMESPACE_END