#include "UpdateProcessor.h"

#include <eden/include/utility/ender_utility.h>

#include <Bifrost/Actor/Actor.h>
#include <Bifrost/Component/Component.h>

USING_BIFROST

CUpdateProcessor::CUpdateProcessor()
{
}

CUpdateProcessor::~CUpdateProcessor()
{
}

bool CUpdateProcessor::Initialize()
{
	do
	{
		m_ProcessList = new ProcessList();
		CHECK_RESULT_BREAK(m_ProcessList);



		return true;
	} while (0);

	Finalize();
	return false;
}

void CUpdateProcessor::Finalize()
{
	if (m_ProcessList)
	{
		for (auto& elem : *m_ProcessList)
		{
			EDENS_NAMESPACE::Delete(elem);
		}

		m_ProcessList->clear();

		EDENS_NAMESPACE::Delete(m_ProcessList);
	}
}

const FUpdateProcessorHandle CUpdateProcessor::SetProcessActor(CActor* _Actor, const FUpdateFunction& _Function)
{
	EObjectType type = EObjectType::OBJECT_TYPE_ACTOR;
	return SetProcessData(_Actor,_Function,type);
}

const FUpdateProcessorHandle CUpdateProcessor::SetProcessComponent(CComponent* _Component, const FUpdateFunction& _Function)
{
	EObjectType type = EObjectType::OBJECT_TYPE_COMPONENT;
	return SetProcessData(_Component, _Function, type);
}

void CUpdateProcessor::DeleteData(const FUpdateProcessorHandle& _Handle)
{
	if (IsInvalidHandle(_Handle)) { return; }

	FUpdateProcessData* data = *_Handle.Handle;
	EDENS_NAMESPACE::Delete(data);

	m_ProcessList->erase(_Handle.Handle);
}

void CUpdateProcessor::UpdateCall(float _DeltaTime) const
{
	for (auto& elem : *m_ProcessList)
	{
		UpdateProcess(_DeltaTime,elem);
	}
}

bool CUpdateProcessor::IsValidHandle(const FUpdateProcessorHandle& _Handle) const
{
	return !IsInvalidHandle(_Handle);
}

bool CUpdateProcessor::IsInvalidHandle(const FUpdateProcessorHandle& _Handle) const
{
	if (!_Handle.IsActive) { return true; }
	return _Handle.Handle == m_ProcessList->end();
}

const FUpdateProcessorHandle CUpdateProcessor::SetProcessData(EDENS_NAMESPACE::CObject* _Object, const FUpdateFunction& _Function, EObjectType _Type)
{
	FUpdateProcessData* data = new FUpdateProcessData();
	if (data == nullptr) { return FUpdateProcessorHandle(); }

	data->ObjectPtr = _Object;
	data->Function = _Function;
	data->ObjectType = _Type;

	m_ProcessList->push_back(data);
	
	FUpdateProcessorHandle handle = {};
	handle.Handle = --m_ProcessList->end();
	handle.IsActive = true;

	return handle;
}

bool CUpdateProcessor::UpdateProcess(float _DeltaTime, const FUpdateProcessData* _Data) const
{
	CHECK_RESULT_FALSE(_Data);

	bool result = false;

	switch (_Data->ObjectType)
	{
	case EObjectType::OBJECT_TYPE_ACTOR:
		{
			const CActor* actor = PCast<const CActor*>(_Data->ObjectPtr);
			CHECK_RESULT_FALSE(actor);
			CHECK_RESULT_FALSE(!actor->IsPendingKill());
			CHECK_RESULT_FALSE(!actor->CanEverUpdate());
			CHECK_RESULT_FALSE(actor->IsActive());
		}

		break;

	case EObjectType::OBJECT_TYPE_COMPONENT:
		{
			const CComponent* component = PCast<const CComponent*>(_Data->ObjectPtr);
			CHECK_RESULT_FALSE(component);
			CHECK_RESULT_FALSE(!component->IsPendingKill());
			CHECK_RESULT_FALSE(component->CanEverUpdate());
			CHECK_RESULT_FALSE(component->IsActive());
		}
		break;

	default:

		break;
	}

	return _Data->Function(_DeltaTime);
}
