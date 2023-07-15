#pragma once

#include <list>

BIFROST_NAMESPACE_BEGIN

struct FActorInitializerBase;
struct FComponentInitializerBase;
class CActor;

class CLevelBase
{
public:

	virtual bool Initialize() { return true; };
	virtual void Finalize();

	virtual void LevelUpdate(float _DeltaTime) {};

	CLevelBase() {};
	~CLevelBase() {};

protected:

	template <class Actor>
	Actor* SpawnActor(const Hash160& _Name,const FActorInitializerBase* _Initializer = nullptr);

	template <class Component>
	Component* CreateComponent(const FComponentInitializerBase* _Initializer = nullptr);

	void DeleteActor(const Hash160& _Name);
	void DeleteActor(const CActor* _Actor);

protected:

	typedef std::map<Hash160,CActor*> FActorList;
	FActorList m_ActorList = {};

};

BIFROST_NAMESPACE_END

#include "LevelBase.inl"