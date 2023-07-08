#include "DebugWindowSubsystem.h"
#include "DebugWindowSubsystemInitializer.h"
#include "DebugWindowSubsystemDefine.h"

#include <Magallanica/Imgui/imgui.h>
#include <Magallanica/Imgui/DirectX12/imgui_impl_win32.h>
#include <Magallanica/Imgui/DirectX12/imgui_impl_dx12.h>
#include <d3d12.h>

#include <Atlantis/RHIProccessor/RHIProcessor.h>
#include <Atlantis/DirectX12/MainDevice/MainDevice.h>

#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Dominator/SubsystemDominator.h>
#include <Bifrost/Subsystem/Rendering/RenderingSubsystem.h>
#include <Atlantis/DirectX12/Command/CommandContext.h>

#include <Magallanica/DebugWindow/DebugWindow.h>

#include <eden/include/utility/ender_utility.h>

USING_MAGALLANICA;

bool CDebugWindowSubsystem::Initialize(const FDebugWindowSubsystemInitializer* _Initializer)
{
    CHECK_RESULT_FALSE(_Initializer);
    const ATLANTIS_NAMESPACE::CRHIProcessor* processor = _Initializer->Processor;
    CHECK_RESULT_FALSE(processor);
    const ATLANTIS_NAMESPACE::CDX12MainDevice* device = processor->GetDevice();
    CHECK_RESULT_FALSE(device);
    ID3D12Device* d3dDevice = device->GetDevice();
    CHECK_RESULT_FALSE(d3dDevice);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(_Initializer->hwnd);


    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    desc.NumDescriptors = 1;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    
    if (FAILED(d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_SRVDesc))))
    {
        return false;
    }

    ImGui_ImplDX12_Init(
        d3dDevice,
        NUM_FRAMES_IN_FLIGHT,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        m_SRVDesc,
        m_SRVDesc->GetCPUDescriptorHandleForHeapStart(),
        m_SRVDesc->GetGPUDescriptorHandleForHeapStart()
    );

    CHECK_RESULT_FALSE(InitializeImpl());

    return true;
}

void CDebugWindowSubsystem::Finalize()
{
    RegisteredFinalize();

    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void CDebugWindowSubsystem::ShowDemoWindow()
{


    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (m_ShowDemoWindow)
    {
        ImGui::ShowDemoWindow(&m_ShowDemoWindow);
    }

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static float f = 0.0f;
        static int counter = 0;
        static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &m_ShowDemoWindow);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &m_ShowAnotherWindow);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (m_ShowAnotherWindow)
    {
        ImGui::Begin("Another Window", &m_ShowAnotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            m_ShowAnotherWindow = false;
        ImGui::End();
    }


}

void CDebugWindowSubsystem::ShowDebugWindow(float _DeltaTime)
{
    // Start the Dear ImGui frame
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //ShowDemoWindow();

    {
        ImGui::Begin("DebugWindowCore", &m_ShowDebugCoreWindow);

        ShowWindowImpl();

        for (auto& elem : m_DebugWindowList)
        {
            elem->ShowWindow(_DeltaTime);
        }

        ImGui::End();
    }

    // Rendering
    ImGui::Render();
}

void CDebugWindowSubsystem::RenderDebugWindow()
{
    BIFROST_NAMESPACE::CRenderingSubsystem* renderer = BIFROST_NAMESPACE::CSubsystemServiceLocator::GetRenderingSubsystemEdit();

    ATLANTIS_NAMESPACE::CCommandContext* context = renderer->GetProcessorEdit()->GetContextEdit();

    ID3D12GraphicsCommandList* list = context->GetCommandList();
    list->SetDescriptorHeaps(1, &m_SRVDesc);
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), list);

}

void CDebugWindowSubsystem::RegistDebugWindow(CDebugWindow* _DebugWindow)
{
    m_DebugWindowList.push_back(_DebugWindow);
}

bool CDebugWindowSubsystem::InitializeImpl()
{
    return true;
}

void CDebugWindowSubsystem::ShowWindowImpl()
{
    for (auto& elem : m_DebugWindowList)
    {
        ImGui::Checkbox(elem->GetWindowName(), elem->OpenWindow());
    }
}

void CDebugWindowSubsystem::RegisteredFinalize()
{
    for (auto& elem : m_DebugWindowList)
    {
        //elem->Finalize();
        EDENS_NAMESPACE::FinalizeObject(elem);
    }
}
