#include <Bifrost/Component/Component.h>

#include <Bifrost/Actor/Actor.h>

#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Updater/UpdateProcessorSubsystem.h>

USING_BIFROST;

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
