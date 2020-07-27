/////////////////////////////////////////////////////////////////////////////
///@file	app_manager.cpp
///@brief	CAppManager		implementation
///@data	2020/06/26
////////////////////////////////////////////////////////////////////////////

//Include Header
#include <rapture/app_manager/app_manager.h>
#include <eden/include/system/window/window.h>
#include <eden/include/system/console/console_window.h>

EDENS_NAMESPACE_USING;

b8 CAppManager::Initialize()
{
	if (!InitializeConsole())
	{
		return false;
	}

	if (!InitializeWindow())
	{
		return false;
	}


	return true;
}


void CAppManager::Finalize()
{
	FinalizeWindow();
	FinalizeConsole();
}


void CAppManager::ApplicationMain()
{

}


b8 CAppManager::InitializeWindow()
{
	m_pWindow = new CWindow();
	if (m_pWindow == nullptr)
	{
		return false;
	}

	return true;
}


void CAppManager::FinalizeWindow()
{

}

b8 CAppManager::InitializeConsole()
{
	return true;
}

void CAppManager::FinalizeConsole()
{

}