#pragma once

//#include <Bifrost/Component/Camera/CameraComponentManager.h>
#include <Bifrost/Subsystem/SubsystemBase/SubsystemBase.h>
#include <Bifrost/Component/Camera/CameraComponentHandle.h>

BIFROST_NAMESPACE_BEGIN

class CCameraComponentManager;

class CCameraComponent;

class CCameraActor;

struct FSubsystemInitializerBase;

class CCameraSubsystem : public CSubsystemBase
{
private:

	//using FHandle = CCameraComponentManager::Handle;
	using FHandle = FCameraComponentManagerHandle;

public:

	bool Initialize(const FSubsystemInitializerBase* _Initializer = nullptr);
	void Finalize();

	const FHandle SetCameraComponent(CCameraComponent* _Component);
	void DeleteComponent(FHandle& _Handle);

	void SetMainCameraComponent(const FHandle& _Handle);
	void SetMainCameraComponent(const CCameraComponent* _Component);
	void SetMainCameraActor(const CCameraActor* _Actor);

	CCameraActor* GetMainCameraActor() const;
	CCameraComponent* GetMainCameraComponent() const;
	const FHandle& GetMainCameraHandle() const;

	CCameraSubsystem();
	~CCameraSubsystem();

private:

	ObjectPtr(CCameraComponentManager) m_CameraManager = nullptr;
	FHandle m_MainCameraHandle = {};

};

BIFROST_NAMESPACE_END