/////////////////////////////////////////////////////////////////////////////
///@file	main.cpp
///@brief	Entry Point
///@data	2020/06/25
////////////////////////////////////////////////////////////////////////////

//Include Header
#include <rapture/app_manager/app_manager.h>
#include <eden/include/system/window/window.h>

#include <Magallanica/Imgui/imgui.h>

using namespace EDENS_NAMESPACE;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmd)
{
	CAppManager& appManager = CAppManager::GetInstance();
	
	if (!appManager.Initialize(hInstance))
	{
		PRINT("AppManager is not initialized.\n");
		AssertMsg(false, "Initialize Failed.\n");
		return cmd;
	}

	appManager.ApplicationMain();

	appManager.Finalize();

	return 0;
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}

	return CAppManager::GetInstance().GetWindow()->MsgProc(hWnd,uMsg,wParam,lParam);
}