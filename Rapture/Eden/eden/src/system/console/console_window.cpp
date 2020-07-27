/////////////////////////////////////////////////////////////////////////////
///@file	console_window.h
///@brief	CConsoleWindow define
///@data	2020/06/26
////////////////////////////////////////////////////////////////////////////

//Include Header
#include <eden/include/system/console/console_window.h>

EDENS_NAMESPACE_BEGIN


void CConsoleWindow::CreateConsoleWindow()
{
	AllocConsole();
	
	FILE* fp = nullptr;

	freopen_s(&m_pFileStream,"CONOUT$", "w", stdout);
	return;
}

void CConsoleWindow::CloseConsoleWindow()
{
	FreeConsole();
	return;
}


EDENS_NAMESPACE_END