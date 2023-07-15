#pragma once

#include <list>
//#include <Bifrost/Component/ComponentManager.h>

BIFROST_NAMESPACE_BEGIN

template <class TComponent> class TComponentManager;

//using ComponentList = typename std::list<TComponent*>;

template <class TComponent>
struct FComponentManagerHandle
{
private:

	friend class TComponentManager<TComponent>;

	//using Handle = typename ComponentList::iterator;

	//typename ComponentList::iterator Handle = {};
	typename std::list<TComponent*>::iterator Handle = {};
	bool IsActive = false;
};


BIFROST_NAMESPACE_END