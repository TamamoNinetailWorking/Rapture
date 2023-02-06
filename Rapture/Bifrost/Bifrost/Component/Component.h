#pragma once

#include <eden/include/Object/Object.h>
#include <Bifrost/Subsystem/Updater/UpdateGroupDefine.h>
#include <Bifrost/Subsystem/Updater/UpdateProcessData.h>

BIFROST_NAMESPACE_BEGIN

struct FComponentInitializerBase;

class CActor;

class CComponent : public EDENS_NAMESPACE::CObject
{
public:

	virtual bool Initialize(const FComponentInitializerBase* _Initializer);
	virtual void Finalize() {};

	virtual void BeginPlay();
	virtual void EndPlay();

	void ReserveKill();

	virtual bool Update(float _DeltaTime) { return false; };

	CComponent() {};
	virtual ~CComponent() {};


	void SetParentActor(CActor* _Parent);
	const CActor* GetActor() const { return m_Parent; };


	bool IsActive() const;
	void SetActive(bool _IsActive);

	bool CanEverUpdate() const;

protected:

	EUpdateGroup m_UpdateGroup = EUpdateGroup::UPDATE_GROUP_INVALID;
	FUpdateProcessorHandle m_ProcessorHandle = {};

	ObjectPtr(CActor) m_Parent = nullptr;

	bool m_CanEverUpdate = false;

private:

	bool m_IsActive = true;

};


BIFROST_NAMESPACE_END