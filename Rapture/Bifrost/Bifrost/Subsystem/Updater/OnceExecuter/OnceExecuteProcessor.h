#pragma once

#include <vector>
#include "OnceExecuteProcessorDefine.h"

BIFROST_NAMESPACE_BEGIN

class CActor;
class CComponent;

struct FOnceExecuteData;

class COnceExecuteProcessor
{
public:

	bool Initialize();
	void Finalize();

	bool SetExecutedActor(CActor* _Actor, const FOnceExecuteFunction& _Function);
	bool SetExecutedComponent(CComponent* _Component, const FOnceExecuteFunction& _Function);

	void Execute();

	COnceExecuteProcessor();
	~COnceExecuteProcessor();

private:

	using ExecuteList = std::vector<FOnceExecuteData*>;

	ObjectPtr(ExecuteList) m_ExecuteList = nullptr;

private:

	void ExecuteListClear();

	bool SetExecutedData(EDENS_NAMESPACE::CObject* _Object, const FOnceExecuteFunction& _Function,EObjectType _Type);

};


BIFROST_NAMESPACE_END