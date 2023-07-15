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
class CUpdateIntervalSubsystem;

#define REGIST_SERVICELOCATOR_PROCEDURE(_HeadStr,_Class) \
public: \
	static const _HeadStr##_Class##* Get##_Class(); \
	static  _HeadStr##_Class##* Get##_Class##Edit(); \

#define REGIST_SERVICELOCATOR_CLASS(_Class) \
	REGIST_SERVICELOCATOR_PROCEDURE(C,_Class) \

#define REGIST_SERVICELOCATOR_INTERFACE(_Class) \
	REGIST_SERVICELOCATOR_PROCEDURE(I,_Class) \


class CSubsystemServiceLocator
{
public:

	static void SetSubsystemDominator(CSubsystemDominator* _Dominator);

	static CSubsystemDominator* GetSubsystemDominator();

public:

	REGIST_SERVICELOCATOR_INTERFACE(ResourceSubsystem);
	REGIST_SERVICELOCATOR_CLASS(UpdateProcessorSubsystem);
	REGIST_SERVICELOCATOR_CLASS(RenderingSubsystem);
	REGIST_SERVICELOCATOR_CLASS(ActorSubsystem);
	REGIST_SERVICELOCATOR_CLASS(CameraSubsystem);
	REGIST_SERVICELOCATOR_CLASS(LightSubsystem);
	REGIST_SERVICELOCATOR_CLASS(UpdateIntervalSubsystem);

#if 0

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

#endif

private:

	static ObjectPtr(CSubsystemDominator) s_Dominator;

};

#undef REGIST_SERVICELOCATOR_INTERFACE
#undef REGIST_SERVICELOCATOR_CLASS
#undef REGIST_SERVICELOCATOR_PROCEDURE

BIFROST_NAMESPACE_END
//ATLANTIS_NAMESPACE_END