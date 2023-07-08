#include "RenderingComponent.h"

#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Rendering/RenderingSubsystem.h>

#include <Atlantis/Material/MaterialInterface.h>

USING_BIFROST;

CRenderingComponent::CRenderingComponent() : Super()
{
    m_UpdateGroup = EUpdateGroup::UPDATE_GROUP_POST_PHYSICS;
    m_CanEverUpdate = true;
}

CRenderingComponent::~CRenderingComponent()
{
    Super::~CComponent();
}

bool CRenderingComponent::Initialize(const FComponentInitializerBase* _Initializer)
{
    do
    {
        CHECK_RESULT_BREAK(Super::Initialize(_Initializer));

        return true;
    } while (0);

    Finalize();
    return false;
}

void CRenderingComponent::Finalize()
{
    Super::Finalize();
}

void CRenderingComponent::BeginPlay()
{
    Super::BeginPlay();

}

void CRenderingComponent::EndPlay()
{

    Super::EndPlay();
}

bool CRenderingComponent::Update(float _DeltaTime)
{
    return RegistRenderer();
}

bool CRenderingComponent::Draw() const
{
    return true;
}

const BIFROST_NAMESPACE::CMeshData* CRenderingComponent::GetMeshData() const
{
    return m_MeshData;
}

const ATLANTIS_NAMESPACE::IMaterialInterface* CRenderingComponent::GetMaterialInterface() const
{
    return m_MaterialInterface;
}

ERenderGroup CRenderingComponent::GetRenderGroup() const
{
    return m_RenderGroup;
}

bool CRenderingComponent::RegistRenderer()
{
    CRenderingSubsystem* subsystem = CSubsystemServiceLocator::GetRenderingSubsystemEdit();
    CHECK_RESULT_FALSE(subsystem);

    if (m_RenderGroup == ERenderGroup::RENDER_GROUP_INVALID) 
    {
        PRINT("CRenderingComponent::RegistRenderer RenderGroup is invalid.");
        return false;
    }

    subsystem->SetRenderObject(m_RenderGroup,this);
    return true;
}