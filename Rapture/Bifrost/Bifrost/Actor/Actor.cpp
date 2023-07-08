#include <Bifrost/Actor/Actor.h>

#include <eden/include/utility/ender_utility.h>

#include <Bifrost/Component/Component.h>

#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Updater/UpdateProcessorSubsystem.h>
#include <Bifrost/Subsystem/Updater/OnceExecuter/OnceExecuteProcessor.h>

USING_BIFROST;

CActor::CActor()
{
	m_ComponentList = new ComponentList();
	m_ComponentList->reserve(10); // ‚Æ‚è‚ ‚¦‚¸‚ÅƒTƒCƒY’¼‘‚«
}

CActor::~CActor()
{
	EDENS_NAMESPACE::Delete(m_ComponentList);
}

bool CActor::Initialize(const FActorInitializerBase* _Initializer)
{
	CUpdateProcessorSubsystem* subsystem = CSubsystemServiceLocator::GetUpdateProcessorSubsystemEdit();
	CHECK_RESULT_FALSE(subsystem);

	FOnceExecuteFunction function = std::bind(&CActor::BeginPlay, this);

	CHECK_RESULT_FALSE(subsystem->SetExecutedActor(EOnceExecuteGroup::ONCE_EXECUTE_GROUP_BEGIN_PLAY, this, function));

	PRINT("Create Actor\n");

	return true;
}

void CActor::Finalize()
{
	for (auto& elem : *m_ComponentList)
	{
		elem->Finalize();
		delete elem;
	}

	m_ComponentList->clear();
}

void CActor::SetHash(const Hash160& _Hash)
{
	m_ActorHash = _Hash;
}

const Hash160& CActor::GetHash() const
{
	return m_ActorHash;
}

bool CActor::AttachComponent(CComponent* _Component)
{
	CHECK_RESULT_FALSE(_Component);
	CHECK_RESULT_FALSE(m_ComponentList);

	m_ComponentList->push_back(_Component);
	_Component->SetParentActor(this);
	return true;
}

bool CActor::DetachComponent(CComponent* _Component)
{
	CHECK_RESULT_FALSE(_Component);
	CHECK_RESULT_FALSE(m_ComponentList);

	bool result = false;

	for (auto itr = m_ComponentList->begin(); itr != m_ComponentList->end(); ++itr)
	{
		if (*itr != _Component)
		{
			continue;
		}

		(*itr)->SetParentActor(nullptr);
		itr = m_ComponentList->erase(itr);
		return result;
	}
	
	return result;
}

void CActor::BeginPlay()
{
	CUpdateProcessorSubsystem* subsystem = CSubsystemServiceLocator::GetUpdateProcessorSubsystemEdit();
	CHECK(subsystem);

	FUpdateFunction function = std::bind(&CActor::Update, this, std::placeholders::_1);
	m_ProcessorHandle = subsystem->SetProcessActor(m_UpdateGroup, this, function);

	PRINT("Called BeginPlay CActor.\n");
}

void CActor::EndPlay()
{
	CUpdateProcessorSubsystem* subsystem = CSubsystemServiceLocator::GetUpdateProcessorSubsystemEdit();
	CHECK(subsystem);

	subsystem->DeleteData(m_UpdateGroup, m_ProcessorHandle);

	Finalize();
}

void CActor::ReserveKill()
{
	for (auto& elem : *m_ComponentList)
	{
		elem->ReserveKill();
	}

	CUpdateProcessorSubsystem* subsystem = CSubsystemServiceLocator::GetUpdateProcessorSubsystemEdit();
	CHECK(subsystem);

	FOnceExecuteFunction function = std::bind(&CActor::EndPlay, this);

	subsystem->SetExecutedActor(EOnceExecuteGroup::ONCE_EXECUTE_GROUP_END_PLAY, this, function);
}

bool CActor::IsActive() const
{
	return m_IsActive;
}

void CActor::SetActive(bool _IsActive)
{
	m_IsActive = _IsActive;
}

bool CActor::CanEverUpdate() const
{
	return m_CanEverUpdate;
}

void CActor::SetActorHash(const Hash160& _Hash)
{
	m_ActorHash = _Hash;
}

const CActor::ComponentList* CActor::GetComponentList() const
{
	return m_ComponentList;
}
