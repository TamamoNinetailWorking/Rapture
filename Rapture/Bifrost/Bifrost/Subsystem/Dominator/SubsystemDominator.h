#pragma once

#include <eden/include/template/NonCopyable.h>

//ATLANTIS_NAMESPACE_BEGIN
BIFROST_NAMESPACE_BEGIN

class IResourceSubsystem;

class CSubsystemDominator : public CNonCopyable
{
public:

	bool Initialize();
	void Finalize();

	
	void SetResourceSubsystem(IResourceSubsystem* _ResSubsystem);

	const IResourceSubsystem* GetResourceSubsystem() const { return m_ResourceSubsystem; };
	IResourceSubsystem* GetResourceSubsystemEdit() const { return m_ResourceSubsystem; };

	CSubsystemDominator() {};
	~CSubsystemDominator() {};

private:

	ObjectPtr(IResourceSubsystem) m_ResourceSubsystem = nullptr;

};

BIFROST_NAMESPACE_END
//ATLANTIS_NAMESPACE_END