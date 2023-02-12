#pragma once

#include <vector>
#include <eden/include/Object/Object.h>
#include <Bifrost/Subsystem/Updater/UpdateGroupDefine.h>
#include <Bifrost/Subsystem/Updater/UpdateProcessData.h>

// Component���܂߂�Bifrost�s�����ȁE�E�E
// �T�[�r�X���P�[�^�[���p�����ɂ��āA�ŏ��ɏ���������Ƃ��A��ԍ��{�̃N���X�Ő�������΂�����̂ł́H
// �����AEden�̃��C�u�����Ƃ��Ă̓��F�Ƃ��Ă��c���̂ł���΁ABifrost����Actor��Component������Ă������C������
// ����ς�Bifrost���炩��

BIFROST_NAMESPACE_BEGIN

class CComponent;

struct FActorInitializerBase;

class CActor : public EDENS_NAMESPACE::CObject 
{
public:

	virtual bool Initialize(const FActorInitializerBase* _Initializer = nullptr);
	virtual void Finalize();

	bool AttachComponent(CComponent* _Component);
	bool DetachComponent(CComponent* _Component);

	template <class Component>
	Component* SearchComponent() const;

	virtual void BeginPlay();
	virtual void EndPlay();

	void ReserveKill();

	virtual bool Update(float _DeltaTime) { return false; };

	void SetHash(const Hash160& _Hash);
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

	bool m_IsActive = true;
	bool m_CanEverUpdate = false;

protected:

	EUpdateGroup m_UpdateGroup = EUpdateGroup::UPDATE_GROUP_PRE_PHYSICS;
	FUpdateProcessorHandle m_ProcessorHandle = {};

protected:

	void SetActorHash(const Hash160& _Hash);

	const ComponentList* GetComponentList() const;


};

BIFROST_NAMESPACE_END

#include "Actor.inl"