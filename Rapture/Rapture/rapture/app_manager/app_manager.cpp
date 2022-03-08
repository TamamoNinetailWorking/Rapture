/////////////////////////////////////////////////////////////////////////////
///@file	app_manager.cpp
///@brief	CAppManager		implementation
///@data	2020/06/26
////////////////////////////////////////////////////////////////////////////

//Include Header
#include <rapture/app_manager/app_manager.h>

// Window System
#include <eden/include/system/window/window.h>
#include <eden/include/system/window/window_def.h>
#include <eden/include/system/console/console_window.h>

// Game Main
#include <rapture/game_manager/game_manager.h>
#include <rapture/game_manager/game_manager_def.h>

EDENS_NAMESPACE_USING;

b8 CAppManager::Initialize(HINSTANCE _hInstance)
{
	if (!InitializeConsole())
	{
		return false;
	}

	if (!InitializeWindow(_hInstance))
	{
		return false;
	}

	if (!InitializeGameManager())
	{
		return false;
	}

	m_pWindow->EdenShowWindow(SW_SHOW);

	return true;
}


void CAppManager::Finalize()
{
	FinalizeGameManager();
	FinalizeWindow();
	FinalizeConsole();
}


void CAppManager::ApplicationMain()
{
	if (m_pWindow == nullptr) { return; }

	while (true)
	{
		m_pWindow->CheckPeekMessage();
		if (m_pWindow->IsMessageQuit())
		{
			break;
		}

		// MainThread Main
		m_pGameManager->GameMain();

	};
}


b8 CAppManager::InitializeWindow(HINSTANCE _hInstance)
{
	m_pWindow = new CWindow();
	if (m_pWindow == nullptr)
	{
		return false;
	}

	FWindowInitializer initializer;
	initializer.Instance = _hInstance;
	initializer.WindowName = "EdenTest";
	initializer.WindowTitle = "EdenTest01";
	initializer.WindowWidth = 1280;
	initializer.WindowHeight = 720;
	//initializer.WindowMode = SW_SHOW;
	initializer.IsFullScreen = false;

	b8 result = m_pWindow->Initialize(&initializer);

	return result;
}


void CAppManager::FinalizeWindow()
{
	if (m_pWindow != nullptr)
	{
		m_pWindow->Finalize();
		delete m_pWindow;
	}
}

b8 CAppManager::InitializeConsole()
{
	m_pConsoleWindow = new CConsoleWindow();
	if (m_pConsoleWindow == nullptr) { return false; }

	m_pConsoleWindow->CreateConsoleWindow();

	return true;
}

void CAppManager::FinalizeConsole()
{	
	if (m_pConsoleWindow != nullptr)
	{
		m_pConsoleWindow->CloseConsoleWindow();
		delete m_pConsoleWindow;
	}
}

b8 CAppManager::InitializeGameManager()
{
	m_pGameManager = new CGameManager();
	if (m_pGameManager == nullptr) { return false; }

	if (m_pWindow == nullptr) { return false; }
	FGameManagerInitializer initializer = {};
	initializer.ViewportWidth = m_pWindow->GetWindowWidth();
	initializer.ViewportHeight = m_pWindow->GetWindowHeight();
	initializer.WindowHandle = m_pWindow->GetHWND();

	return m_pGameManager->Initialize(&initializer);
}

void CAppManager::FinalizeGameManager()
{
	if (m_pGameManager != nullptr)
	{
		m_pGameManager->Finalize();
		delete m_pGameManager;
	}
}
