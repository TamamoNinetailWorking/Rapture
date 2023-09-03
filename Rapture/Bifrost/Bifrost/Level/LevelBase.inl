#include <Bifrost/Actor/ActorInitializerBase.h>
#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Actor/ActorSubsystem.h>

#include <string>

BIFROST_NAMESPACE_BEGIN

template <class Actor>
Actor* CLevelBase::SpawnActor(const Hash160& _Name, const FActorInitializerBase* _Initializer)
{
	CActorSubsystem* subsystem = CSubsystemServiceLocator::GetActorSubsystemEdit();
	if (!subsystem) { return nullptr; };

	//FActorInitializerBase* initializer = nullptr;
	Actor* actor = subsystem->CreateActor<Actor>(_Initializer);
	if (actor == nullptr)
	{
		PRINT("LevelBase::SpawnActor() : Failed to create actor.\n");
		return nullptr;
	}

	Hash160 hash = _Name;
	if (m_ActorList.find(_Name) != m_ActorList.end())
	{
		std::string name = {};
		name = RHash160(_Name);
		name = name + "02";
		hash = CHash160(name);
	}

	actor->SetHash(hash);
	m_ActorList[hash] = actor;

	return actor;
}

template <class Component>
Component* CLevelBase::CreateComponent(const FComponentInitializerBase* _Initializer)
{
	Component* component = new Component;
	if (!component) { return nullptr; };
	if (!component->Initialize(_Initializer)) { return nullptr; };

	return component;
}

BIFROST_NAMESPACE_END