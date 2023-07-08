#pragma once

#include <Bifrost/Subsystem/SubsystemBase/SubsystemBase.h>
#include <Bifrost/Component/Light/LightComponentHandle.h>

BIFROST_NAMESPACE_BEGIN

class CLightComponent;

class CLightComponentManager;

class CLightSubsystem : public CSubsystemBase
{
private:

	using FHandle = FLightComponentManagerHandle;

public:

	bool Initialize(const FSubsystemInitializerBase* _Initializer = nullptr);
	void Finalize();

	const FHandle SetLightComponent(CLightComponent* _Component);
	void DeleteComponent(FHandle& _Handle);

	void SetMainLightComponent(const FHandle& _Handle);
	void SetMainLightComponent(const CLightComponent* _Component);

	CLightComponent* GetMainLightComponent() const;
	const FHandle& GetMainLightHandle() const;

	CLightSubsystem();
	~CLightSubsystem();

private:

	ObjectPtr(CLightComponentManager) m_LightManager = nullptr;
	
	// フォワードレンダリングのテスト用
	FHandle m_MainLightHandle = {};
};

BIFROST_NAMESPACE_END