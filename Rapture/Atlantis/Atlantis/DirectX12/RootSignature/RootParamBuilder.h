#pragma once


ATLANTIS_NAMESPACE_BEGIN

struct FRootParameterInitializer;

class CRootParamBuilder
{
public:

	bool Initialize(const FRootParameterInitializer* _Initializer);
	void Finalize();

	CRootParamBuilder();
	~CRootParamBuilder();

private:

	class Impl;
	ObjectPtr(Impl) m_Impl = nullptr;

};


ATLANTIS_NAMESPACE_END