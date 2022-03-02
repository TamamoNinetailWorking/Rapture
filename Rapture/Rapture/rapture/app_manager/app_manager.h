/////////////////////////////////////////////////////////////////////////////
///@file	app_manager.h
///@brief	CAppManager		define
///@data	2020/06/26
////////////////////////////////////////////////////////////////////////////
#ifndef __APP_MANAGER_H__
#define __APP_MANAGER_H__

//Include Header
#include <eden/include/template/singleton.h>

EDENS_NAMESPACE_BEGIN

class CWindow;
class CConsoleWindow;

EDENS_NAMESPACE_END

/*
*	アプリケーションマネージャー
*/
class CAppManager : public EDENS_NAMESPACE::CSingleton<CAppManager>
{
protected:

	friend class EDENS_NAMESPACE::CSingleton<CAppManager>;

public:

	b8 Initialize(HINSTANCE _hInstance);

	void Finalize();

	void ApplicationMain();

	EDENS_NAMESPACE::CWindow* GetWindow() const { return m_pWindow; };

protected:

	EDENS_NAMESPACE::CWindow* m_pWindow = nullptr;
	EDENS_NAMESPACE::CConsoleWindow* m_pConsoleWindow = nullptr;


	b8 InitializeWindow(HINSTANCE _hInstance);
	void FinalizeWindow();

	b8 InitializeConsole();
	void FinalizeConsole();

};

#endif