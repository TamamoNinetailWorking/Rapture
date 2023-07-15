#include "DebugPostEffect.h"

#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Actor/ActorSubsystem.h>

#include <Bifrost/Component/Rendering/PostEffect/BloomPostEffectComponent.h>
#include <Bifrost/Actor/PostEffecctActor/PostEffectActor.h>

#include <Bifrost/RenderTarget/RenderTarget.h>
#include <Atlantis/DirectX12/RenderTargetView/RenderTargetView.h>
#include <Atlantis/DirectX12/RenderTargetView/RenderTargetViewResource.h>

#include <Magallanica/Imgui/imgui.h>

USING_BIFROST;

CDebugPostEffect::CDebugPostEffect() : CDebugWindow()
{
    m_WindowName = "DebugPostEffect";
    m_OpenWindow = false;
}

void CDebugPostEffect::ShowWindowImpl(float _DeltaTime)
{
    CActorSubsystem* Subsystem = CSubsystemServiceLocator::GetActorSubsystemEdit();
    CHECK(Subsystem);

    CPostEffectActor* PPActor = Subsystem->SearchActor<CPostEffectActor>(CHash160("PostEffectActor01"));
    CHECK(PPActor);

    {
        ImGui::Text("%s", RHash160(PPActor->GetHash()));
    }

    ///
    /// RenderTargetを表示するデバッグ機能を作成する
    /// 
    
    CBloomEffectComponent* BEC = PPActor->SearchComponent<CBloomEffectComponent>();
    CHECK(BEC);

    ImTextureID TexID = {};
    uint32 Count = 0;
    for (auto& elem : BEC->GetRTVBuffers())
    {
        uint64 Handle = elem->GetRenderTargetView()->GetRenderTargetView()->GetShaderResourceView()->GetGPUDescriptorHandleForHeapStart().ptr;

        TexID = RCast<void*>(Handle);
        
        ImGui::Text("RTV%02d",Count);

        ImGui::Image(TexID, ImVec2(320, 180));
        
        ++Count;
    }


 //   {
	//	CBloomEffectComponent* BloomComponent = PPActor->GetComponent<CBloomEffectComponent>();
	//	CHECK(BloomComponent);

	//	ImGui::Text("Bloom");
	//	ImGui::SliderFloat("BloomThreshold", &BloomComponent->GetBloomThreshold(), 0.0f, 1.0f);
	//	ImGui::SliderFloat("BloomIntensity", &BloomComponent->GetBloomIntensity(), 0.0f, 1.0f);
	//	ImGui::SliderFloat("BloomSaturation", &BloomComponent->GetBloomSaturation(), 0.0f, 1.0f);
	//	ImGui::SliderFloat("BloomBlurSigma", &BloomComponent->GetBloomBlurSigma(), 0.0f, 10.0f);
	//}
}