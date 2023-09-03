#include "PostEffectActor.h"
#include "PostEffectActorInitializer.h"

#include <Bifrost/Component/Rendering/PostEffect/BloomPostEffectComponentInitializer.h>
#include <Bifrost/Component/Rendering/PostEffect/BloomPostEffectComponent.h>

USING_BIFROST;

CPostEffectActor::CPostEffectActor() : Super()
{
}

CPostEffectActor::~CPostEffectActor()
{
	Super::~CActor();
}

bool CPostEffectActor::Initialize(const FActorInitializerBase* _Initializer)
{
	do
	{
		CHECK_RESULT_FALSE(Super::Initialize(_Initializer));

		const FPostEffectActorInitializer* initializer = PCast<const FPostEffectActorInitializer*>(_Initializer);

		{
			FBloomPostEffectComponentInitializer bloomInit = {};
			bloomInit.QuadPolygonPsoName = initializer->QuadPolygonPsoName;
			bloomInit.BrightnessPsoName = initializer->BrightnessPsoName;
			bloomInit.BlurPsoName = initializer->BlurPsoName;
			bloomInit.FetchColorPsoName = initializer->FetchColorPsoName;
			
			m_BloomPostEffectComponent = new CBloomEffectComponent();
			CHECK_RESULT_FALSE(m_BloomPostEffectComponent);
			CHECK_RESULT_FALSE(m_BloomPostEffectComponent->Initialize(&bloomInit));

			AttachComponent(m_BloomPostEffectComponent);
		}

		return true;
	} while (0);

	Finalize();
	return false;
}

void CPostEffectActor::Finalize()
{
	Super::Finalize();
}

void CPostEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void CPostEffectActor::EndPlay()
{
	Super::EndPlay();
}

bool CPostEffectActor::Update(float _DeltaTime)
{
	return Super::Update(_DeltaTime);
}
