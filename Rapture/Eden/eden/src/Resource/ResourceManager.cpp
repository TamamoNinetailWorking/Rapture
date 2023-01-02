#include "eden/include/Resource/ResourceManager.h"

#include <eden/include/Resource/Resource.h>
#include <eden/include/Resource/ResourceInitializer.h>

#include "eden/include/utility/ender_utility.h"

//#include <eden/include/utility/StringUtility.h>

EDENS_NAMESPACE_USING;

bool CResourceManager::Initialize()
{
	m_ResourceList.clear();
	return true;
}


void CResourceManager::Finalize()
{
	for (auto& elem : m_ResourceList)
	{
		if (elem.ResPtr == nullptr) { continue; }

		if (elem.RefCount <= 0 && elem.ResPtr != nullptr)
		{
			PRINT("CResourceManager::Finalize Error.\n ResourceManager has Resource,but RefCount is bigger than [Zero].\n");
		}

		elem.ResPtr->Finalize();
		elem.RefCount = 0;

		Delete(elem.ResPtr);
	}

	m_ResourceList.clear();
}


//FResourceHandle CResourceManager::CreateResource(const FResourceInitializerBase* _Initializer)
//{
//	FResourceHandle handle = {};
//	if (_Initializer == nullptr) { return handle; }
//
//	FManagementResource resource = {};
//
//	//resource.ResPtr = new CResource();
//	resource.ResPtr = CreateResourceObject();
//	if (resource.ResPtr == nullptr) { return handle; }
//
//	if (!resource.ResPtr->Initialize(_Initializer)) { return handle; };
//	++resource.RefCount;
//
//	m_ResourceList.push_back(resource);
//
//	handle.iterator = --m_ResourceList.end();
//
//	return handle;
//}


FResourceHandle CResourceManager::SearchCreateResource(const FResourceInitializerBase* _Initializer)
{
	FResourceHandle handle = {};
	if (_Initializer == nullptr) { return handle; }

	handle = SearchResourceHandle(_Initializer->Name);

	if(!IsInvalidHandle(handle))
	{
		++handle.iterator->RefCount;
		return handle;
	}

	
	return CreateResourceImpl(_Initializer);
}


void CResourceManager::AddRef(const FResourceHandle& _Handle) const
{
	if (IsInvalidHandle(_Handle)) { return; }
	++_Handle.iterator->RefCount;
}


void CResourceManager::DeleteResource(const FResourceHandle& _Handle)
{
	if (IsInvalidHandle(_Handle)) { return; }

	sint32& count = _Handle.iterator->RefCount;
	--count;

	if (count <= 0)
	{
		CResource*& ptr = _Handle.iterator->ResPtr;
		Delete(ptr);

		m_ResourceList.erase(_Handle.iterator);
	}
}


void CResourceManager::DeleteResource(const Hash160& _Hash)
{
	FResourceHandle handle = SearchResourceHandle(_Hash);
	DeleteResource(handle);
}

bool CResourceManager::IsValidHandle(const FResourceHandle& _Handle) const
{
	return !IsInvalidHandle(_Handle);
}


bool CResourceManager::IsInvalidHandle(const FResourceHandle& _Handle) const
{
	if (!_Handle.isActive) { return true; }

	// 無意味なiteratorがまず許されない・・・
	//if (_Handle == GetInvalidHandle()) { return true; }

	// endを先に判定しようとすると、まずListの整合性をチェックしようとするので、無意味なiteratorを持っているとexception判定になる
	return _Handle.iterator == m_ResourceList.end();
	//return _Handle.iterator == m_ResourceList.end() || _Handle == INVALID_RESOURCE_HANDLE;
	//return _Handle == GetInvalidHandle();
}

FResourceHandle CResourceManager::GetInvalidHandle() const
{
	return INVALID_RESOURCE_HANDLE;
}


const CResource* CResourceManager::SearchResource(const FResourceHandle& _Handle) const
{
	++_Handle.iterator->RefCount;
	return _Handle.iterator->ResPtr;
}


const CResource* CResourceManager::SearchResource(const Hash160& _Hash)
{
	FResourceHandle handle = SearchResourceHandle(_Hash);
	return SearchResource(handle);
}


FResourceHandle CResourceManager::SearchResourceHandle(const Hash160& _Hash)
{
	FResourceHandle handle = GetInvalidHandle();
	//handle.iterator = m_ResourceList.end();

	for (auto itr = m_ResourceList.begin(); itr != m_ResourceList.end(); ++itr)
	{
		if (itr->ResPtr == nullptr)
		{
			PRINT("CResourceManager::SearchResourceHandle >> ResourceManager has invalid resource.(RefCount is Zero.)\n");
			continue;
		}

		if (itr->ResPtr->GetName() == _Hash)
		{
			handle.iterator = itr;
			handle.isActive = true;

			++handle.iterator->RefCount;
			break;
		}
	}

	return handle;
}

FResourceHandle CResourceManager::SearchResourceHandle(const FResourceInitializerBase* _Initializer)
{
	return SearchResourceHandle(_Initializer->Name);
}

