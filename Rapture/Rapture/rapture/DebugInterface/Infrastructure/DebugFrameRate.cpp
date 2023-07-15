#include "DebugFrameRate.h"

#include <Magallanica/Imgui/imgui.h>

#include <Bifrost/Subsystem/UpdateInterval/UpdateIntervalSubsystem.h>
#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>

#include <string>

USING_BIFROST;

using namespace std;

extern double g_TimerResult[3];

CDebugFrameRate::CDebugFrameRate() : CDebugWindow()
{
    m_WindowName = "DebugFrameRate";
    m_OpenWindow = true;
}

void CDebugFrameRate::ShowWindowImpl(float _DeltaTime)
{

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    const CUpdateIntervalSubsystem* subsystem = CSubsystemServiceLocator::GetUpdateIntervalSubsystem();
 
    float frameTime = SCast<float>(subsystem->GetExecuteTime());
    //float frameTime = _DeltaTime;
    
    {
        static float frameInterval = 0.f;
        static float fps = 0.f;
        static uint16 count = 0;
        constexpr uint16 countLimit = 5;
        {
            if (count >= countLimit)
            {
                frameInterval = frameTime * 1000.0f;
                fps = 1.0f / frameTime;
            }
            ++count;
        }

        ImGui::Text("IntervalTimer avarage %.3f ms/frame (%.1f FPS)", frameInterval, fps);
    }

    static double Time = 0.0;

    Time += frameTime;
    ImGui::Text("Current Time %.3f", Time);

    ImGui::Text("");

    {
        static bool StopWatchFlag = false;

        string ButtonLabel = {};
        const string StartLabel = "Start";
        const string StopLabel = "Stop";

        static float StopWatchTime = 0.f;

        if (StopWatchFlag)
        {
            ButtonLabel = StartLabel;
            StopWatchTime += frameTime;
        }
        else
        {
            ButtonLabel = StopLabel;
        }

        ImGui::Checkbox(ButtonLabel.c_str(), &StopWatchFlag);
        ImGui::SameLine();
        ImGui::Text("%.3f", StopWatchTime);

        if (ImGui::Button("ResetTimer"))
        {
            StopWatchTime = 0.f;
        }
    }

    ImGui::Text("");

    {
        ImGui::Text("Execution Total    %lf", g_TimerResult[0]);
        ImGui::Text("Game               %lf", g_TimerResult[1]);
        ImGui::Text("Render             %f", g_TimerResult[2]);
    }

    {
        if (Time >= 3600)
        {
            Time = 0.0;
        }
    }

    //{
    //    if (frameTime * 1000.0f >= (1000.0f / 60 + 10))
    //    {
    //        PRINT("FrameTimeSpike %.3f\n", frameTime * 1000.f);
    //    }
    //}
}
