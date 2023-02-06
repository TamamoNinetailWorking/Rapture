#pragma once

#include <eden/include/template/NonCopyable.h>

//ATLANTIS_NAMESPACE_BEGIN
BIFROST_NAMESPACE_BEGIN

class IResourceSubsystem;
class CUpdateProcessorSubsystem;
class CRenderingSubsystem;
class CActorSubsystem;

class CSubsystemDominator : public CNonCopyable
{
public:

	bool Initialize();
	void Finalize();

	
	void SetResourceSubsystem(IResourceSubsystem* _ResSubsystem);
	void SetUpdaterSubsystem(CUpdateProcessorSubsystem* _UpdaterSubsystem);
	void SetRenderingSubsystem(CRenderingSubsystem* _RenderingSubsystem);
	void SetActorSubsystem(CActorSubsystem* _ActorSubsystem);

	const IResourceSubsystem* GetResourceSubsystem() const { return m_ResourceSubsystem; };
	IResourceSubsystem* GetResourceSubsystemEdit() const { return m_ResourceSubsystem; };

	const CUpdateProcessorSubsystem* GetUpdaterSubsystem() const { return m_UpdaterSubsystem; };
	CUpdateProcessorSubsystem* GetUpdaterSubsystemEdit() const { return m_UpdaterSubsystem; };

	const CRenderingSubsystem* GetRenderingSubsystem() const { return m_RenderingSubsystem; };
	CRenderingSubsystem* GetRenderingSubsystemEdit() const { return m_RenderingSubsystem; };

	const CActorSubsystem* GetActorSubsystem() const { return m_ActorSubsystem; }
	CActorSubsystem* GetActorSubsystemEdit() const { return m_ActorSubsystem; }

	CSubsystemDominator() {};
	~CSubsystemDominator() {};

private:

	ObjectPtr(IResourceSubsystem) m_ResourceSubsystem = nullptr;
	ObjectPtr(CUpdateProcessorSubsystem) m_UpdaterSubsystem = nullptr;
	ObjectPtr(CRenderingSubsystem) m_RenderingSubsystem = nullptr;
	ObjectPtr(CActorSubsystem) m_ActorSubsystem = nullptr;

};

BIFROST_NAMESPACE_END
//ATLANTIS_NAMESPACE_END