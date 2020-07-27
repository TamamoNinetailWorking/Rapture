/////////////////////////////////////////////////////////////////////////////
///@file	console_window.h
///@brief	CConsoleWindow define
///@data	2020/06/26
////////////////////////////////////////////////////////////////////////////
#ifndef __CONSOLE_WINDOW_H__
#define __CONSOLE_WINDOW_H__

//Include Header
#include <stdio.h>

EDENS_NAMESPACE_BEGIN

/*
*	CUI�E�B���h�E�����N���X
*/
class CConsoleWindow
{
public:

	void CreateConsoleWindow();


	void CloseConsoleWindow();

private:

	FILE* m_pFileStream = nullptr;

};


EDENS_NAMESPACE_END

#endif