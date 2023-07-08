#include "SubsystemServiceLocator.h"

#include <Bifrost/Subsystem/Dominator/SubsystemDominator.h>

USING_BIFROST;

ObjectPtr(CSubsystemDominator) CSubsystemServiceLocator::s_Dominator = nullptr;

void CSubsystemServiceLocator::SetSubsystemDominator(CSubsystemDominator* _Dominator)
{
	s_Dominator = _Dominator;
}

CSubsystemDominator* CSubsystemServiceLocator::GetSubsystemDominator()
{
	return s_Dominator;
}

#define REGIST_FUNCTION_SERVICELOCATOR_PROCEDURE(_HeadStr,_Class) \
	const _HeadStr##_Class##* CSubsystemServiceLocator::Get##_Class() \
	{ \
		return s_Dominator->Get##_Class##(); \
	} \
	\
	 _HeadStr##_Class##* CSubsystemServiceLocator::Get##_Class##Edit() \
	{ \
		return s_Dominator->Get##_Class##Edit(); \
	} \

#define REGIST_FUNCTION_SERVICELOCATOR_CLASS(_Class) \
	REGIST_FUNCTION_SERVICELOCATOR_PROCEDURE(C,_Class) \

#define REGIST_FUNCTION_SERVICELOCATOR_INTERFACE(_Class) \
	REGIST_FUNCTION_SERVICELOCATOR_PROCEDURE(I,_Class) \



REGIST_FUNCTION_SERVICELOCATOR_INTERFACE(ResourceSubsystem);
REGIST_FUNCTION_SERVICELOCATOR_CLASS(UpdateProcessorSubsystem);
REGIST_FUNCTION_SERVICELOCATOR_CLASS(RenderingSubsystem);
REGIST_FUNCTION_SERVICELOCATOR_CLASS(ActorSubsystem);
REGIST_FUNCTION_SERVICELOCATOR_CLASS(CameraSubsystem);
REGIST_FUNCTION_SERVICELOCATOR_CLASS(LightSubsystem);
REGIST_FUNCTION_SERVICELOCATOR_CLASS(UpdateIntervalSubsystem);



#undef REGIST_FUNCTION_SERVICELOCATOR_INTERFACE
#undef REGIST_FUNCTION_SERVICELOCATOR_CLASS
#undef REGIST_FUNCTION_SERVICELOCATOR_PROCEDURE

#if 0
const IResourceSubsystem* CSubsystemServiceLocator::GetResourceSubsystem()
{
	return s_Dominator->GetResourceSubsystem();
}

IResourceSubsystem* CSubsystemServiceLocator::GetResourceSubsystemEdit()
{
	return s_Dominator->GetResourceSubsystemEdit();
}

const CUpdateProcessorSubsystem* CSubsystemServiceLocator::GetUpdaterSubsystem()
{
	return s_Dominator->GetUpdateProcessorSubsystem();
}

CUpdateProcessorSubsystem* CSubsystemServiceLocator::GetUpdaterSubsystemEdit()
{
	return s_Dominator->GetUpdateProcessorSubsystemEdit();
}

const CRenderingSubsystem* CSubsystemServiceLocator::GetRenderingSubsystem()
{
	return s_Dominator->GetRenderingSubsystem();
}

CRenderingSubsystem* CSubsystemServiceLocator::GetRenderingSubsystemEdit()
{
	return s_Dominator->GetRenderingSubsystemEdit();
}

const CActorSubsystem* CSubsystemServiceLocator::GetActorSubsystem()
{
	return s_Dominator->GetActorSubsystem();;
}

CActorSubsystem* CSubsystemServiceLocator::GetActorSubsystemEdit()
{
	return s_Dominator->GetActorSubsystemEdit();
}

const CCameraSubsystem* CSubsystemServiceLocator::GetCameraSubsystem()
{
	return s_Dominator->GetCameraSubsystem();
}

CCameraSubsystem* CSubsystemServiceLocator::GetCameraSubsystemEdit()
{
	return s_Dominator->GetCameraSubsystemEdit();;
}

const CLightSubsystem* CSubsystemServiceLocator::GetLightSubsystem()
{
	return s_Dominator->GetLightSubsystem();
}

CLightSubsystem* CSubsystemServiceLocator::GetLightSubsystemEdit()
{
	return s_Dominator->GetLightSubsystemEdit();
}
#endif