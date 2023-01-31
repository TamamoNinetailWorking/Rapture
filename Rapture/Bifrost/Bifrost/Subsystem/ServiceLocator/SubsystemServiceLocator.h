#pragma once


//ATLANTIS_NAMESPACE_BEGIN
BIFROST_NAMESPACE_BEGIN

class CSubsystemDominator;
class IResourceSubsystem;
class CUpdateProcessorSubsystem;
class CRenderingSubsystem;

class CSubsystemServiceLocator
{
public:

	static void SetSubsystemDominator(CSubsystemDominator* _Dominator);

	static CSubsystemDominator* GetSubsystemDominator();

	static const IResourceSubsystem* GetResourceSubsystem();
	static IResourceSubsystem* GetResourceSubsystemEdit();

	static const CUpdateProcessorSubsystem* GetUpdaterSubsystem();
	static CUpdateProcessorSubsystem* GetUpdaterSubsystemEdit();

	static const CRenderingSubsystem* GetRenderingSubsystem();
	static CRenderingSubsystem* GetRenderingSubsystemEdit();

private:

	static ObjectPtr(CSubsystemDominator) s_Dominator;

};


BIFROST_NAMESPACE_END
//ATLANTIS_NAMESPACE_END