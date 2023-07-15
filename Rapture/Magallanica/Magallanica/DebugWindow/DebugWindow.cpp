#include "DebugWindow.h"

#include <Magallanica/Imgui/imgui.h>

USING_MAGALLANICA

bool CDebugWindow::Initialize(const FDebugWindowInitializer* _Initializer)
{
	return true;
}

void CDebugWindow::Finalize()
{
}

void CDebugWindow::ShowWindow(float _DeltaTime)
{
	if (m_OpenWindow)
	{
		ImGui::Begin(m_WindowName.c_str(), &m_OpenWindow);

		ShowWindowImpl(_DeltaTime);

		ImGui::End();
	}
}

bool* CDebugWindow::OpenWindow()
{
	return &m_OpenWindow;
}

const char* CDebugWindow::GetWindowName() const
{
	return m_WindowName.c_str();
}
