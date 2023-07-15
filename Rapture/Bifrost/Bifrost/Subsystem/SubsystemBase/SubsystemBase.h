#pragma once


BIFROST_NAMESPACE_BEGIN

struct FSubsystemInitializerBase;

class CSubsystemBase
{
public:

	virtual bool Initialize(const FSubsystemInitializerBase* _Initializer = nullptr) = 0;
	virtual void Finalize() = 0;

	CSubsystemBase() {};
	virtual ~CSubsystemBase() {};

};

BIFROST_NAMESPACE_END