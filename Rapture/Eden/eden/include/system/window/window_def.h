#pragma once

EDENS_NAMESPACE_BEGIN

/*
*	GUIウィンドウ生成のための初期化構造体
*/
struct FWindowInitializer
{

	HINSTANCE Instance = nullptr;//ウィンドウ構造体
	const c8* WindowName = nullptr;//ウィンドウ名
	const c8* WindowTitle = nullptr;//ウィンドウタイトル
	//s32 WindowMode = SW_HIDE; // ウィンドウモード
	s32 WindowWidth = 0;//ウィンドウの横幅
	s32 WindowHeight = 0;//ウィンドウの高さ
	b8 IsFullScreen = false;//フルスクリーン設定

};

EDENS_NAMESPACE_END