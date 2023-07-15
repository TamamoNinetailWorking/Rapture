#pragma once

#include <list>
#include "ComponentManagerHandleBase.h"

BIFROST_NAMESPACE_BEGIN

//class CComponent;

#define M_HANDLE typename FComponentManagerHandle<TComponent>

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

	M_HANDLE RegistComponent(TComponent* _Component);
	void DeleteComponent(M_HANDLE& _Handle);

	const TComponent* SearchComponent(const M_HANDLE& _Handle);
	TComponent* SearchComponentEdit(const M_HANDLE& _Handle);

	TComponentManager() {};
	~TComponentManager() {};

private:

	using TCMHandle = M_HANDLE;

private:

	ObjectPtr(ComponentList) m_ComponentList = nullptr;

	void ListFinalize();

	bool IsHandleInvalid(const M_HANDLE& _Handle);

};

#undef M_HANDLE

BIFROST_NAMESPACE_END

#include "ComponentManager.inl"