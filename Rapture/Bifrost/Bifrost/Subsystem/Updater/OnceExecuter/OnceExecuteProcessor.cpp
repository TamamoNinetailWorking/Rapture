#include "OnceExecuteProcessor.h"

#include<eden/include/utility/ender_utility.h>

#include <Bifrost/Actor/Actor.h>
#include <Bifrost/Component/Component.h>

USING_BIFROST

COnceExecuteProcessor::COnceExecuteProcessor()
{
}

COnceExecuteProcessor::~COnceExecuteProcessor()
{
}

bool COnceExecuteProcessor::Initialize()
{
	do
	{
		m_ExecuteList = new ExecuteList();
		CHECK_RESULT_BREAK(m_ExecuteList);

		m_ExecuteList->resize(ONCE_EXECUTER_LIST_NUM);

		return true;
	} while (0);

	Finalize();
	return false;
}

void COnceExecuteProcessor::Finalize()
{
	if (m_ExecuteList)
	{
		for (auto& elem : *m_ExecuteList)
		{
			EDENS_NAMESPACE::Delete(elem);
		}

		m_ExecuteList->clear();

		EDENS_NAMESPACE::Delete(m_ExecuteList);
	}
}

bool COnceExecuteProcessor::SetExecutedActor(CActor* _Actor, const FOnceExecuteFunction& _Function)
{
	EObjectType type = EObjectType::OBJECT_TYPE_ACTOR;
	return SetExecutedData(_Actor, _Function, type);;
}

bool COnceExecuteProcessor::SetExecutedComponent(CComponent* _Component, const FOnceExecuteFunction& _Function)
{
	EObjectType type = EObjectType::OBJECT_TYPE_COMPONENT;
	return SetExecutedData(_Component, _Function, type);
}

void COnceExecuteProcessor::Execute()
{
	for (auto& elem : *m_ExecuteList)
	{

#define EXECUTE_ELEMENT_IS_INVALID(param) \
	if(param == nullptr) \
	{\
		 continue;\
	}\

		EXECUTE_ELEMENT_IS_INVALID(elem);

		switch (elem->ObjectType)
		{
		case EObjectType::OBJECT_TYPE_ACTOR:
			{
				const CActor* actor = PCast<const CActor*>(elem->ObjectPtr);
				EXECUTE_ELEMENT_IS_INVALID(actor);
			}
			break;

		case EObjectType::OBJECT_TYPE_COMPONENT:
			{
				const CComponent* component = PCast<const CComponent*>(elem->ObjectPtr);
				EXECUTE_ELEMENT_IS_INVALID(component);
			}
			break;

		default:
			break;
		}

		elem->Function();
	}

#undef EXECUTE_ELEMENT_IS_INVALID

	ExecuteListClear();
}

void COnceExecuteProcessor::ExecuteListClear()
{
	m_ExecuteList->resize(0);
}

bool COnceExecuteProcessor::SetExecutedData(EDENS_NAMESPACE::CObject* _Object, const FOnceExecuteFunction& _Function, EObjectType _Type)
{
	FOnceExecuteData* data = nullptr;
	data = new FOnceExecuteData();
	CHECK_RESULT_FALSE(data);

	data->ObjectPtr = _Object;
	data->Function = _Function;
	data->ObjectType = _Type;

	m_ExecuteList->push_back(data);

	return true;
}