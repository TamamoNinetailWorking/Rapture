#pragma once

#include <vector>
#include <eden/include/Object/Object.h>

// Componentも含めてBifrost行きかな・・・
// サービスロケーターを継承式にして、最初に初期化するとき、一番根本のクラスで生成すればいけるのでは？
// ただ、Edenのライブラリとしての特色としてを残すのであれば、BifrostからActorやComponentを作ってもいい気がする
// やっぱりBifrostからかな

BIFROST_NAMESPACE_BEGIN

class CComponent;

class CActor : public EDENS_NAMESPACE::CObject 
{
public:

	virtual bool Initialize() { return false; };
	virtual void Finalize();

	bool AttachComponent(CComponent* _Component);
	bool DetachComponent(CComponent* _Component);

	virtual void BeginPlay() {};
	virtual void EndPlay() {};

	virtual bool Update(float _DeltaTime) { return false; };

	const Hash160& GetHash() const;


	bool IsActive() const;
	void SetActive(bool _IsActive);

	bool CanEverUpdate() const;

	CActor();
	virtual ~CActor();

private:

	Hash160 m_ActorHash = {};

	using ComponentList = std::vector<CComponent*>;

	ObjectPtr(ComponentList) m_ComponentList = nullptr;

	bool m_IsActive = false;
	bool m_CanEverUpdate = false;

protected:

	void SetActorHash(const Hash160& _Hash);

	const ComponentList* GetComponentList() const;


};

BIFROST_NAMESPACE_END