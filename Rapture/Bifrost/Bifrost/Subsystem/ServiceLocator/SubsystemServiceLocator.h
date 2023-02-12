#pragma once


//ATLANTIS_NAMESPACE_BEGIN
BIFROST_NAMESPACE_BEGIN

class CSubsystemDominator;
class IResourceSubsystem;
class CUpdateProcessorSubsystem;
class CRenderingSubsystem;
class CActorSubsystem;
class CCameraSubsystem;
class CLightSubsystem;

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

	static const CActorSubsystem* GetActorSubsystem();
	static CActorSubsystem* GetActorSubsystemEdit();

	static const CCameraSubsystem* GetCameraSubsystem();
	static CCameraSubsystem* GetCameraSubsystemEdit();

	static const CLightSubsystem* GetLightSubsystem();
	static CLightSubsystem* GetLightSubsystemEdit();

private:

	static ObjectPtr(CSubsystemDominator) s_Dominator;

};


BIFROST_NAMESPACE_END
//ATLANTIS_NAMESPACE_END