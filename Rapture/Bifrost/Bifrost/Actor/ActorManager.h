#pragma once

/*

ループのイメージは


Physics Update
Input Update
Actor Update

Rendering


まずはUnityのLifecycle準拠だろうか



ComponentのUpdate順のことなどを考えると、
UnrealEngineのTickGroup制御がいいのかもしれない

TickGroup、VisibilityとRendererComponentの有無で
更新処理毎に処理と描画リストを作る

ActorManager->Update
	GroupSeparation
		PrePhysicsUpdate
		PhysicsUpdate
		PostPhysicsUpdate
		RenderUpdate

Componentのことも考えると、UpdateSubsystemみたいなものが必要かもしれない
	>> ComponentをManagerで管理すると、Actorと二重でComponentListを持ちそう


描画処理は切り分けておけば、レンダリングスレッドとしてDrawコールを行う部分を切り分けやすいかもしれない
GPUにデータ転送していないものについてはレンダリングスレッド側にコピーする必要があったりするだろうが

*/

#include <list>

BIFROST_NAMESPACE_BEGIN

class CActor;

struct FActorInitializerBase;

class CActorManager
{
public:

	bool Initialize();
	void Finalize();

	
	//void Update(float _DeltaTime);

	template <class Actor>
	CActor* CreateActor(const FActorInitializerBase* _Initializer);

	CActor* SearchActor(const Hash160& _Name);

	void DeleteActor(const Hash160& _Name);
	void DeleteActor(const CActor* _Actor);

	void DestroyActors();

	CActorManager();
	~CActorManager();

private:
	
	using ActorList = std::list<CActor*>;

	ObjectPtr(ActorList) m_ActorList = nullptr;


	void ListFinalize();

};

BIFROST_NAMESPACE_END

#include "ActorManager.inl"