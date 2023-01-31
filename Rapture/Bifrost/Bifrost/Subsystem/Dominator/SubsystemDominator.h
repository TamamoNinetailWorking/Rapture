#pragma once

#include <eden/include/template/NonCopyable.h>

//ATLANTIS_NAMESPACE_BEGIN
BIFROST_NAMESPACE_BEGIN

class IResourceSubsystem;
class CUpdateProcessorSubsystem;
class CRenderingSubsystem;

class CSubsystemDominator : public CNonCopyable
{
public:

	bool Initialize();
	void Finalize();

	
	void SetResourceSubsystem(IResourceSubsystem* _ResSubsystem);
	void SetUpdaterSubsystem(CUpdateProcessorSubsystem* _UpdaterSubsystem);
	void SetRenderingSubsystem(CRenderingSubsystem* _RenderingSubsystem);

	const IResourceSubsystem* GetResourceSubsystem() const { return m_ResourceSubsystem; };
	IResourceSubsystem* GetResourceSubsystemEdit() const { return m_ResourceSubsystem; };

	const CUpdateProcessorSubsystem* GetUpdaterSubsystem() const { return m_UpdaterSubsystem; };
	CUpdateProcessorSubsystem* GetUpdaterSubsystemEdit() const { return m_UpdaterSubsystem; };

	const CRenderingSubsystem* GetRenderingSubsystem() const { return m_RenderingSubsystem; };
	CRenderingSubsystem* GetRenderingSubsystemEdit() const { return m_RenderingSubsystem; };

	CSubsystemDominator() {};
	~CSubsystemDominator() {};

private:

	ObjectPtr(IResourceSubsystem) m_ResourceSubsystem = nullptr;
	ObjectPtr(CUpdateProcessorSubsystem) m_UpdaterSubsystem = nullptr;
	ObjectPtr(CRenderingSubsystem) m_RenderingSubsystem = nullptr;

};

BIFROST_NAMESPACE_END
//ATLANTIS_NAMESPACE_END