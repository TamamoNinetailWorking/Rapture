#include "CameraSubsystem.h"

#include <eden/include/utility/ender_utility.h>

#include <Bifrost/Component/Camera/CameraComponentManager.h>
#include <Bifrost/Component/Camera/CameraComponent.h>
#include <Bifrost/Actor/CameraActor/CameraActor.h>

USING_BIFROST;

CCameraSubsystem::CCameraSubsystem()
{
}

CCameraSubsystem::~CCameraSubsystem()
{
}

bool CCameraSubsystem::Initialize()
{
	do
	{
		m_CameraManager = new CCameraComponentManager();
		CHECK_RESULT_BREAK(m_CameraManager);

		CHECK_RESULT_BREAK(m_CameraManager->Initialize());

		return true;
		
	} while (0);

	Finalize();
	return false;
}

void CCameraSubsystem::Finalize()
{
	EDENS_NAMESPACE::FinalizeObject(m_CameraManager);
}

const CCameraSubsystem::FHandle CCameraSubsystem::SetCameraComponent(CCameraComponent* _Component)
{
	if (!m_CameraManager) { return FHandle(); }
	return m_CameraManager->RegistComponent(_Component);
}

void CCameraSubsystem::DeleteComponent(FHandle& _Handle)
{
	CHECK(m_CameraManager);
	m_CameraManager->DeleteComponent(_Handle);
}

void CCameraSubsystem::SetMainCameraComponent(const FHandle& _Handle)
{
	m_MainCameraHandle = _Handle;
}

void CCameraSubsystem::SetMainCameraComponent(const CCameraComponent* _Component)
{
	CHECK(_Component);
	//m_MainLightHandle = _Component->GetHandle();
	SetMainCameraComponent(_Component->GetHandle());
}

void CCameraSubsystem::SetMainCameraActor(const CCameraActor* _Actor)
{
	CHECK(_Actor);
	auto* Component = _Actor->GetCameraComponent();
	CHECK(Component);
	//m_MainLightHandle = Component->GetHandle();
	SetMainCameraComponent(Component);
}

CCameraActor* CCameraSubsystem::GetMainCameraActor() const
{
	auto* Component = GetMainCameraComponent();
	if (!Component) { return nullptr; }
	return DCast<CCameraActor*>(Component->GetParentActorEdit());
}

CCameraComponent* CCameraSubsystem::GetMainCameraComponent() const
{
	if (!m_CameraManager) { return nullptr; }
	auto* Component = m_CameraManager->SearchComponentEdit(m_MainCameraHandle);
	return Component;
}

const CCameraSubsystem::FHandle& CCameraSubsystem::GetMainCameraHandle() const
{
	return m_MainCameraHandle;
}
