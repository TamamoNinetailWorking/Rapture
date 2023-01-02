#pragma once

/*
*    　>>Handleは別のヘッダーに定義し直したほうがいいかもしれない。Handleだけで持つときにオーバーヘッドが大きくなる
      >>ResourceクラスのInitializerも似た理由で別定義したほうが良さそう
* 
*/

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


// ハンドルで直接iteratorを持つのは比較処理が面倒になる
// 有効・無効判定が簡単には行えない
// >> iteratorが空の状態で比較しようとするとexceptionが飛んでくる

typedef std::list<FManagementResource> FResourceList;

struct FResourceHandle
{
private:
	friend class CResourceManager;

	typedef FResourceList::iterator Handle;

	Handle iterator = {};

	bool isActive = false;

public:

	bool operator==(const FResourceHandle& _handle) const
	{
		return iterator == _handle.iterator;
	}

	bool operator !=(const FResourceHandle& _handle) const
	{
		return *this != _handle;
	}
};

static const FResourceHandle INVALID_RESOURCE_HANDLE = FResourceHandle();

EDENS_NAMESPACE_END