#pragma once

#include <Bifrost/Subsystem/Updater/UpdateGroupDefine.h>
#include "UpdateProcessData.h"

BIFROST_NAMESPACE_BEGIN

class CUpdateProcessor;

class CActor;
class CComponent;

class CUpdateProcessorSubsystem
{
public:

	bool Initialize();
	void Finalize();

	void ProcessorUpdate(float _DeltaTime);

	const FUpdateProcessorHandle SetProcessActor(EUpdateGroup _UpdateGroup,CActor* _Actor, const FUpdateFunction& _Function);
	const FUpdateProcessorHandle SetProcessComponent(EUpdateGroup _UpdateGroup, CComponent* _Component, const FUpdateFunction& _Function);

	void DeleteData(EUpdateGroup _UpdateGroup,const FUpdateProcessorHandle& _Handle);

	CUpdateProcessor* GetPrePhysicsProcessor() const;
	CUpdateProcessor* GetPhysicsDuringProcessor() const;
	CUpdateProcessor* GetPostPhysicsProcessor() const;

	CUpdateProcessorSubsystem();
	~CUpdateProcessorSubsystem();

private:

	using Updater = std::array<CUpdateProcessor*, EUpdateGroup::UPDATE_GROUP_NUM>;

	Updater m_Updater = {};

	CUpdateProcessor* GetProcessor(EUpdateGroup _Type) const;

};


BIFROST_NAMESPACE_END