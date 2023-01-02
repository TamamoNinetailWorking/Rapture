#pragma once


EDENS_NAMESPACE_BEGIN

//template <class TResource>
//FResourceHandle CResourceManager::SearchCreateResource(const FResourceInitializerBase* _Initializer)
//{
//	FResourceHandle handle = {};
//	if (_Initializer == nullptr) { return handle; }
//
//	handle = SearchResourceHandle(_Initializer);
//
//	if (!IsInvalidHandle(handle))
//	{
//		++handle.iterator->RefCount;
//		return handle;
//	}
//
//
//	return CreateResource<TResource>(_Initializer);
//}

template <class TResource>
FResourceHandle CResourceManager::CreateResource(const FResourceInitializerBase* _Initializer)
{
	FResourceHandle handle = GetInvalidHandle();
	if (_Initializer == nullptr) { return handle; }

	FManagementResource resource = {};

	TResource* resPtr = new TResource();
	//resource.ResPtr = new TResource();
	//resource.ResPtr = CreateResourceObject();
	if (resPtr == nullptr) { return handle; }

	if (!resPtr->Initialize(_Initializer)) { return handle; };
	
	++resource.RefCount;
	resource.ResPtr = PCast<CResource*>(resPtr);

	m_ResourceList.push_back(resource);

	handle.iterator = --m_ResourceList.end();
	handle.isActive = true;

	return handle;
}

EDENS_NAMESPACE_END