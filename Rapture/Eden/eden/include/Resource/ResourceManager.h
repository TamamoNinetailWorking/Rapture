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

	FResourceHandle SearchCreateResource(const FResourceInitializerBase* _Initializer);
	const CResource* SearchResource(const FResourceHandle& _Handle) const;
	const CResource* SearchResource(const Hash160& _Hash);

	void DeleteResource(const FResourceHandle& _Handle);
	void DeleteResource(const Hash160& _Hash);

	bool IsInvalidHandle(const FResourceHandle& _Handle) const;



	CResourceManager() {};
	~CResourceManager() {};

protected:

	virtual CResource* CreateResourceObject() = 0;

private:

	FResourceHandle CreateResource(const FResourceInitializerBase* _Initializer);
	FResourceHandle SearchResourceHandle(const Hash160& _Hash);

	FResourceList m_ResourceList = {};

};


EDENS_NAMESPACE_END