#include <Bifrost/Component/Component.h>

#include <Bifrost/Actor/Actor.h>

#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Updater/UpdateProcessorSubsystem.h>
#include <Bifrost/Subsystem/Updater/OnceExecuter/OnceExecuteProcessor.h>

USING_BIFROST;

bool CComponent::Initialize(const FComponentInitializerBase* _Initializer)
{
	CUpdateProcessorSubsystem* subsystem = CSubsystemServiceLocator::GetUpdaterSubsystemEdit();
	CHECK_RESULT_FALSE(subsystem);

	FOnceExecuteFunction function = std::bind(&CComponent::BeginPlay, this);

	return subsystem->SetExecutedComponent(EOnceExecuteGroup::ONCE_EXECUTE_GROUP_BEGIN_PLAY,this,function);
}

void CComponent::BeginPlay()
{
    //-----------------------
	// 
	// ����AComponent��BeginPlay�ɂ�点�Đe���ĂԌ`�ɂ���
	// >>����Update�̊֐��|�C���^�̕������|�����[�t�B�Y���ɑΉ��ł��邩�������̂ŁA
	// �@�R���\�[���Ƃ��ň�x�e�X�g�����ق���������������Ȃ�
	// 
	//-----------------------
    CUpdateProcessorSubsystem* subsystem = CSubsystemServiceLocator::GetUpdaterSubsystemEdit();
    CHECK(subsystem);

    FUpdateFunction function = std::bind(&CComponent::Update, this, std::placeholders::_1);
    m_ProcessorHandle = subsystem->SetProcessComponent(m_UpdateGroup, this, function);
}

void CComponent::EndPlay()
{
	CUpdateProcessorSubsystem* subsystem = CSubsystemServiceLocator::GetUpdaterSubsystemEdit();
	CHECK(subsystem);

	subsystem->DeleteData(m_UpdateGroup, m_ProcessorHandle);

	Finalize();
}

void CComponent::ReserveKill()
{
	CUpdateProcessorSubsystem* subsystem = CSubsystemServiceLocator::GetUpdaterSubsystemEdit();
	CHECK(subsystem);

	FOnceExecuteFunction function = std::bind(&CComponent::EndPlay, this);

	subsystem->SetExecutedComponent(EOnceExecuteGroup::ONCE_EXECUTE_GROUP_END_PLAY, this, function);
}

void CComponent::SetParentActor(CActor* _Parent)
{
	m_Parent = _Parent;	
}

bool CComponent::IsActive() const
{
	if (!m_Parent) { return m_IsActive; }
	if (!m_Parent->IsActive()) { return false; }
	return m_IsActive;
}

void CComponent::SetActive(bool _IsActive)
{
	m_IsActive = _IsActive;
}

bool CComponent::CanEverUpdate() const
{
	return m_CanEverUpdate;
}
