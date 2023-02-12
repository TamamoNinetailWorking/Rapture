#include "LightSubsystem.h"

#include <eden/include/utility/ender_utility.h>

#include <Bifrost/Component/Light/LightComponentManager.h>
#include <Bifrost/Component/Light/LightComponent.h>


USING_BIFROST;

CLightSubsystem::CLightSubsystem()
{
}

CLightSubsystem::~CLightSubsystem()
{
}

bool CLightSubsystem::Initialize()
{
	do
	{
		m_LightManager = new CLightComponentManager();
		CHECK_RESULT_BREAK(m_LightManager);

		CHECK_RESULT_BREAK(m_LightManager->Initialize());

		return true;

	} while (0);

	Finalize();
	return false;
}

void CLightSubsystem::Finalize()
{
	EDENS_NAMESPACE::FinalizeObject(m_LightManager);
}

const CLightSubsystem::FHandle CLightSubsystem::SetLightComponent(CLightComponent* _Component)
{
	if (!m_LightManager) { return FHandle(); }
	return m_LightManager->RegistComponent(_Component);
}

void CLightSubsystem::DeleteComponent(FHandle& _Handle)
{
	CHECK(m_LightManager);
	m_LightManager->DeleteComponent(_Handle);
}

void CLightSubsystem::SetMainLightComponent(const FHandle& _Handle)
{
	m_MainLightHandle = _Handle;
}

void CLightSubsystem::SetMainLightComponent(const CLightComponent* _Component)
{
	CHECK(_Component);
	SetMainLightComponent(_Component->GetHandle());
}

CLightComponent* CLightSubsystem::GetMainLightComponent() const
{
	if (!m_LightManager) { return nullptr; }
	return m_LightManager->SearchComponentEdit(m_MainLightHandle);
}

const CLightSubsystem::FHandle& CLightSubsystem::GetMainLightHandle() const
{
	return m_MainLightHandle;
}


