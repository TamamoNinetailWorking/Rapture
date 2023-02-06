#pragma once

#include <Bifrost/Subsystem/Updater/UpdateGroupDefine.h>
#include <Bifrost/Subsystem/Updater/OnceExecuteGroup.h>
#include "UpdateProcessData.h"
#include <Bifrost/Subsystem/Updater/OnceExecuter/OnceExecuteProcessorDefine.h>

BIFROST_NAMESPACE_BEGIN

class CUpdateProcessor;
class COnceExecuteProcessor;

class CActor;
class CComponent;

class CUpdateProcessorSubsystem
{
public:

	bool Initialize();
	void Finalize();

	void BeginPlayExecute();
	void EndPlayExecute();

	void ProcessorUpdate(float _DeltaTime);

	const FUpdateProcessorHandle SetProcessActor(EUpdateGroup _UpdateGroup,CActor* _Actor, const FUpdateFunction& _Function);
	const FUpdateProcessorHandle SetProcessComponent(EUpdateGroup _UpdateGroup, CComponent* _Component, const FUpdateFunction& _Function);

	bool SetExecutedActor(EOnceExecuteGroup _ExecuteGroup, CActor* _Actor, const FOnceExecuteFunction& _Function);
	bool SetExecutedComponent(EOnceExecuteGroup _ExecuteGroup, CComponent* _Component, const FOnceExecuteFunction& _Function);

	void DeleteData(EUpdateGroup _UpdateGroup,const FUpdateProcessorHandle& _Handle);

	CUpdateProcessor* GetPrePhysicsProcessor() const;
	CUpdateProcessor* GetPhysicsDuringProcessor() const;
	CUpdateProcessor* GetPostPhysicsProcessor() const;

	COnceExecuteProcessor* GetBeginPlayExecuter() const;
	COnceExecuteProcessor* GetEndPlayExecuter() const;

	CUpdateProcessorSubsystem();
	~CUpdateProcessorSubsystem();

private:

	using Updater = std::array<CUpdateProcessor*, EUpdateGroup::UPDATE_GROUP_NUM>;

	Updater m_Updater = {};

	using OnceExecuter = std::array<COnceExecuteProcessor*, EOnceExecuteGroup::ONCE_EXECUTE_GROUP_NUM>;

	OnceExecuter m_OnceExecuter = {};

private:

	CUpdateProcessor* GetProcessor(EUpdateGroup _Type) const;

	COnceExecuteProcessor* GetOnceExecuter(EOnceExecuteGroup _Type) const;

	void Execute(EOnceExecuteGroup _Type);

};


BIFROST_NAMESPACE_END