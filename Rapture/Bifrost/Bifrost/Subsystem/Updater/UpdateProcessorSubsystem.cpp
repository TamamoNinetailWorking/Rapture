#include "UpdateProcessorSubsystem.h"
#include "UpdateProcessor.h"

#include <eden/include/utility/ender_utility.h>

USING_BIFROST;

CUpdateProcessorSubsystem::CUpdateProcessorSubsystem()
{
}

CUpdateProcessorSubsystem::~CUpdateProcessorSubsystem()
{
}

bool CUpdateProcessorSubsystem::Initialize()
{
	do
	{
		for (uint32 index = 0; index < EUpdateGroup::UPDATE_GROUP_NUM; ++index)
		{
			auto& data = m_Updater[index];
			data = new CUpdateProcessor();
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
	for (auto& elem : m_Updater)
	{
		EDENS_NAMESPACE::FinalizeObject(elem);
	}
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
	return m_Updater.at(_Type);
}
