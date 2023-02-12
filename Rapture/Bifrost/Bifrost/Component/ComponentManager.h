#pragma once

#include <list>
#include "ComponentManagerHandleBase.h"

BIFROST_NAMESPACE_BEGIN

//class CComponent;

template <class TComponent>
class TComponentManager
{
private:

	using ComponentList = typename std::list<TComponent*>;

public:

	//template <class TComponent>
	//struct FComponentManagerHandle
	//{
	//private:

	//	friend class TComponentManager<TComponent>;

	//	//using Handle = typename ComponentList::iterator;

	//	typename ComponentList::iterator Handle = {};
	//	bool IsActive = false;
	//};

	bool Initialize();
	void Finalize();

	typename FComponentManagerHandle<TComponent> RegistComponent(TComponent* _Component);
	void DeleteComponent(typename FComponentManagerHandle<TComponent>& _Handle);

	const TComponent* SearchComponent(const typename FComponentManagerHandle<TComponent>& _Handle);
	TComponent* SearchComponentEdit(const typename FComponentManagerHandle<TComponent>& _Handle);

	TComponentManager() {};
	~TComponentManager() {};

private:

	using TCMHandle = typename FComponentManagerHandle<TComponent>;

private:

	ObjectPtr(ComponentList) m_ComponentList = nullptr;

	void ListFinalize();

	bool IsHandleInvalid(const typename FComponentManagerHandle<TComponent>& _Handle);

};

BIFROST_NAMESPACE_END

#include "ComponentManager.inl"