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
*	@brief CUIウィンドウ生成クラス
*/
class CConsoleWindow
{
public:

	/**
	 * @brief CUIウィンドウの生成
	 * 
	 */
	void CreateConsoleWindow();

	/**
	 * @brief CUIウィンドウの破棄
	 * 
	 */
	void CloseConsoleWindow();

private:

	/** 入力のためのストリーム出力先 */
	FILE* m_pFileStream = nullptr;

};


EDENS_NAMESPACE_END

#endif