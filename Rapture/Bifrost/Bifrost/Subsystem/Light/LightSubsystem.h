#pragma once

#include <Bifrost/Component/Light/LightComponentHandle.h>

BIFROST_NAMESPACE_BEGIN

class CLightComponent;

class CLightComponentManager;

class CLightSubsystem
{
private:

	using FHandle = FLightComponentManagerHandle;

public:

	bool Initialize();
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
	FHandle m_MainLightHandle = {};
};

BIFROST_NAMESPACE_END