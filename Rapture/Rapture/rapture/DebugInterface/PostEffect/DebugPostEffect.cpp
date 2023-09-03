#include "DebugPostEffect.h"

#include <Magallanica/Subsystem/DebugSubsystemServiceLocator.h>
#include <Magallanica/Subsystem/DebugWindowSubsystem.h>

#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Actor/ActorSubsystem.h>

#include <Bifrost/Component/Rendering/PostEffect/BloomPostEffectComponent.h>
#include <Bifrost/Actor/PostEffecctActor/PostEffectActor.h>

#include <Bifrost/RenderTarget/RenderTarget.h>
#include <Atlantis/DirectX12/RenderTargetView/RenderTargetView.h>
#include <Atlantis/DirectX12/RenderTargetView/RenderTargetViewResource.h>

#include <Magallanica/Imgui/imgui.h>

#include <vector>

USING_MAGALLANICA;
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
    
    CBloomEffectComponent* BEC = PPActor->SearchComponent<CBloomEffectComponent>();
    CHECK(BEC);

    CDebugWindowSubsystem* DebugSubsystem = CDebugSubsystemServiceLocator::GetDebugWindowSubsystem();
    CHECK(DebugSubsystem);

    static bool IsFirst = true;
    static std::vector<uint32> SRVHandles = {};
    if (IsFirst)
    {
        SRVHandles.reserve(64);
    }

    ImTextureID TexID = {};
    //uint32 Count = 0;
    for (uint32 Count = 0; Count < BEC->GetRTVBuffers().size();++Count)
    {
        const auto& elem = BEC->GetRTV(Count);

        auto RTV = elem->GetRenderTargetView()->GetRenderTargetView();
        ID3D12DescriptorHeap* Heap = RTV->GetShaderResourceView();
        CHECK(Heap);

        if (IsFirst)
        {
            uint32 Index = DebugSubsystem->SetSRV(RTV->GetResource());
            SRVHandles.push_back(Index);
        }

        uint64 Handle = DebugSubsystem->GetSRVGPUHandle(SRVHandles[Count]);

        TexID = RCast<ImTextureID>(Handle);

        
        ImGui::Text("RTV%02d",Count);

        ImGui::Image(TexID, ImVec2(320, 180));
    }

    if (IsFirst)
    {
        IsFirst = false;
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