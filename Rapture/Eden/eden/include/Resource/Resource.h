#pragma once


EDENS_NAMESPACE_BEGIN

class CResource
{
public:

	struct FInitializerBase {};

	virtual bool Initialize(const FInitializerBase* _Initializer) = 0;
	virtual void Finalize() = 0;


	const Hash160& GetName() const { return m_ResourceName; };

	CResource() {};
	virtual ~CResource();

protected:

	Hash160 m_ResourceName = {};

};


EDENS_NAMESPACE_END