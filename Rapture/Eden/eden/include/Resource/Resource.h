#pragma once

//#include "ResourceInitializer.h"

EDENS_NAMESPACE_BEGIN

struct FResourceInitializerBase;

class CResource
{
public:

	virtual bool Initialize(const FResourceInitializerBase* _Initializer) { return false; };
	virtual void Finalize() {};


	const Hash160& GetName() const { return m_ResourceName; };

	CResource() {};
	virtual ~CResource();

protected:

	Hash160 m_ResourceName = {};

};


EDENS_NAMESPACE_END