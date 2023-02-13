#pragma once

BIFROST_NAMESPACE_BEGIN

class CSubsystemDominator;
class CResourceManagementSubsystem;

class IResourceSubsystem;

class CUpdateProcessorSubsystem;
class CRenderingSubsystem;
class CActorSubsystem;
class CCameraSubsystem;
class CLightSubsystem;

BIFROST_NAMESPACE_END

/*
*	ゲーム処理を統括するマネージャー
*/
class CGameManager
{
public:

	b8 Initialize(struct FGameManagerInitializer* _Initializer);

	void Finalize();

	void GameMain();

private:

	void GameUpdate();

	void Render();

private:

	ObjectPtr(BIFROST_NAMESPACE::CSubsystemDominator) m_SubsystemDominator = nullptr;
	ObjectPtr(BIFROST_NAMESPACE::CResourceManagementSubsystem) m_ResourceSubsystem = nullptr;

	ObjectPtr(BIFROST_NAMESPACE::IResourceSubsystem) m_ResSystemInterface = nullptr;

	ObjectPtr(BIFROST_NAMESPACE::CUpdateProcessorSubsystem) m_UpdaterSubsystem = nullptr;
	ObjectPtr(BIFROST_NAMESPACE::CRenderingSubsystem) m_RenderingSubsystem = nullptr;

	ObjectPtr(BIFROST_NAMESPACE::CActorSubsystem) m_ActorSubsytem = nullptr;

	ObjectPtr(BIFROST_NAMESPACE::CCameraSubsystem) m_CameraSubsystem = nullptr;

	ObjectPtr(BIFROST_NAMESPACE::CLightSubsystem) m_LightSubsystem = nullptr;

};