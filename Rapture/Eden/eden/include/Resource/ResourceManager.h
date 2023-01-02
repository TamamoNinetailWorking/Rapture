#pragma once

#include "ResourceManagerPreDefine.h"

EDENS_NAMESPACE_BEGIN

//class CResource;
struct FResourceInitializerBase;


// Note 参照カウンタ周りの機能は改修が必要になりそう
class CResourceManager
{
public:

	bool Initialize();
	void Finalize();

	//template <class TResource>
	FResourceHandle SearchCreateResource(const FResourceInitializerBase* _Initializer);
	const CResource* SearchResource(const FResourceHandle& _Handle) const;
	const CResource* SearchResource(const Hash160& _Hash);

	FResourceHandle SearchResourceHandle(const Hash160& _Hash);

	void AddRef(const FResourceHandle& _Handle) const;

	void DeleteResource(const FResourceHandle& _Handle);
	void DeleteResource(const Hash160& _Hash);

	bool IsValidHandle(const FResourceHandle& _Handle) const;
	bool IsInvalidHandle(const FResourceHandle& _Handle) const;

	FResourceHandle GetInvalidHandle() const;

	CResourceManager() {};
	~CResourceManager() {};

protected:

	virtual FResourceHandle CreateResourceImpl(const FResourceInitializerBase* _Initializer) = 0;

	template <class TResource>
	FResourceHandle CreateResource(const FResourceInitializerBase* _Initializer);

private:

	// inlineファイル内で使用することを想定
	FResourceHandle SearchResourceHandle(const FResourceInitializerBase* _Initializer);

	FResourceList m_ResourceList = {};

};


EDENS_NAMESPACE_END

#include "ResourceManager.inl"