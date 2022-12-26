#pragma once

#include <list>

EDENS_NAMESPACE_BEGIN

class CResource;

struct FManagementResource
{
private:
	friend class CResourceManager;
	
	ObjectPtr(CResource) ResPtr = nullptr;
	sint32 RefCount = 0;
};

typedef std::list<FManagementResource> FResourceList;

struct FResourceHandle
{
private:
	friend class CResourceManager;

	typedef FResourceList::iterator Handle;

	Handle iterator = {};

public:

	bool operator==(const FResourceHandle& _handle)
	{
		return iterator == _handle.iterator;
	}

	bool operator !=(const FResourceHandle& _handle)
	{
		return *this != _handle;
	}
};

EDENS_NAMESPACE_END