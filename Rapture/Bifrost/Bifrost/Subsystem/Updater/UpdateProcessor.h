#pragma once

#include <list>
#include "UpdateProcessData.h"

EDENS_NAMESPACE_BEGIN

class CObject;

EDENS_NAMESPACE_END


BIFROST_NAMESPACE_BEGIN

class CActor;
class CComponent;

struct FUpdateProcessData;

class CUpdateProcessor
{
public:

	bool Initialize();
	void Finalize();

	const FUpdateProcessorHandle SetProcessActor(CActor* _Actor, const FUpdateFunction& _Function);
	const FUpdateProcessorHandle SetProcessComponent(CComponent* _Component, const FUpdateFunction& _Function);

	void DeleteData(const FUpdateProcessorHandle& _Handle);

	void UpdateCall(float _DeltaTime) const;

	bool IsValidHandle(const FUpdateProcessorHandle& _Handle) const;
	bool IsInvalidHandle(const FUpdateProcessorHandle& _Handle) const;

	CUpdateProcessor();
	~CUpdateProcessor();

private:

	using ProcessList = std::list<FUpdateProcessData*>;

	ObjectPtr(ProcessList) m_ProcessList = nullptr;

	const FUpdateProcessorHandle SetProcessData(EDENS_NAMESPACE::CObject* _Object, const FUpdateFunction& _Function, EObjectType _Type);

	bool UpdateProcess(float _DeltaTime, const FUpdateProcessData* _Data) const;

};


BIFROST_NAMESPACE_END