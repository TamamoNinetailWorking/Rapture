#include "UpdateProcessorSubsystem.h"
#include "UpdateProcessor.h"
#include <Bifrost/Subsystem/Updater/OnceExecuter/OnceExecuteProcessor.h>

#include <eden/include/utility/ender_utility.h>


USING_BIFROST;

CUpdateProcessorSubsystem::CUpdateProcessorSubsystem()
{
}

CUpdateProcessorSubsystem::~CUpdateProcessorSubsystem()
{
}

bool CUpdateProcessorSubsystem::Initialize(const FSubsystemInitializerBase* _Initializer)
{
	do
	{
		for (uint32 index = 0; index < UNumCast(EUpdateGroup::UPDATE_GROUP_NUM); ++index)
		{
			auto& data = m_Updater[index];
			data = new CUpdateProcessor();
			CHECK_RESULT_BREAK(data);
			CHECK_RESULT_BREAK(data->Initialize());
		}

		for (uint32 index = 0; index < UNumCast(EOnceExecuteGroup::ONCE_EXECUTE_GROUP_NUM); ++index)
		{
			auto& data = m_OnceExecuter[index];
			data = new COnceExecuteProcessor();
			CHECK_RESULT_BREAK(data);
			CHECK_RESULT_BREAK(data->Initialize());
		}

		return true;
	} while (0);

	Finalize();
	return false;
}

void CUpdateProcessorSubsystem::Finalize()
{
	for (auto& elem : m_OnceExecuter)
	{
		EDENS_NAMESPACE::FinalizeObject(elem);
	}

	for (auto& elem : m_Updater)
	{
		EDENS_NAMESPACE::FinalizeObject(elem);
	}
}

void CUpdateProcessorSubsystem::BeginPlayExecute()
{
	Execute(EOnceExecuteGroup::ONCE_EXECUTE_GROUP_BEGIN_PLAY);
}

void CUpdateProcessorSubsystem::EndPlayExecute()
{
	Execute(EOnceExecuteGroup::ONCE_EXECUTE_GROUP_END_PLAY);
}

void CUpdateProcessorSubsystem::ProcessorUpdate(float _DeltaTime)
{
	for (auto& elem : m_Updater)
	{
		elem->UpdateCall(_DeltaTime);
	}
}

const FUpdateProcessorHandle CUpdateProcessorSubsystem::SetProcessActor(EUpdateGroup _UpdateGroup, CActor* _Actor, const FUpdateFunction& _Function)
{
	CUpdateProcessor* processor = GetProcessor(_UpdateGroup);
	if (!processor) { return FUpdateProcessorHandle(); };

	return processor->SetProcessActor(_Actor,_Function);
}

const FUpdateProcessorHandle CUpdateProcessorSubsystem::SetProcessComponent(EUpdateGroup _UpdateGroup, CComponent* _Component, const FUpdateFunction& _Function)
{
	CUpdateProcessor* processor = GetProcessor(_UpdateGroup);
	if (!processor) { return FUpdateProcessorHandle(); };

	return processor->SetProcessComponent(_Component,_Function);
}

bool CUpdateProcessorSubsystem::SetExecutedActor(EOnceExecuteGroup _ExecuteGroup, CActor* _Actor, const FOnceExecuteFunction& _Function)
{
	COnceExecuteProcessor* executer = GetOnceExecuter(_ExecuteGroup);
	CHECK_RESULT_FALSE(executer);

	return executer->SetExecutedActor(_Actor,_Function);
}

bool CUpdateProcessorSubsystem::SetExecutedComponent(EOnceExecuteGroup _ExecuteGroup, CComponent* _Component, const FOnceExecuteFunction& _Function)
{
	COnceExecuteProcessor* executer = GetOnceExecuter(_ExecuteGroup);
	CHECK_RESULT_FALSE(executer);

	return executer->SetExecutedComponent(_Component, _Function);
}

void CUpdateProcessorSubsystem::DeleteData(EUpdateGroup _UpdateGroup, const FUpdateProcessorHandle& _Handle)
{
	CUpdateProcessor* processor = GetProcessor(_UpdateGroup);
	CHECK(processor);

	return processor->DeleteData(_Handle);
}

CUpdateProcessor* CUpdateProcessorSubsystem::GetPrePhysicsProcessor() const
{
	return GetProcessor(EUpdateGroup::UPDATE_GROUP_PRE_PHYSICS);
}

CUpdateProcessor* CUpdateProcessorSubsystem::GetPhysicsDuringProcessor() const
{
	return GetProcessor(EUpdateGroup::UPDATE_GROUP_PHYSICS_DURING);
}

CUpdateProcessor* CUpdateProcessorSubsystem::GetPostPhysicsProcessor() const
{
	return GetProcessor(EUpdateGroup::UPDATE_GROUP_POST_PHYSICS);
}

CUpdateProcessor* CUpdateProcessorSubsystem::GetProcessor(EUpdateGroup _Type) const
{
	return m_Updater.at(UNumCast(_Type));
}

COnceExecuteProcessor* CUpdateProcessorSubsystem::GetOnceExecuter(EOnceExecuteGroup _Type) const
{
	return m_OnceExecuter.at(UNumCast(_Type));
}

void CUpdateProcessorSubsystem::Execute(EOnceExecuteGroup _Type)
{
	COnceExecuteProcessor* executer = GetOnceExecuter(_Type);
	CHECK(executer);

	executer->Execute();
}
