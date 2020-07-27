/////////////////////////////////////////////////////////////////////////////
///@file	window.h
///@brief	CWindow define
///@data	2020/06/26
////////////////////////////////////////////////////////////////////////////
#ifndef __WINDOW_H__
#define __WINDOW_H__

EDENS_NAMESPACE_BEGIN

/*
*	GUIウィンドウ生成クラス
*/
class CWindow
{
public:


	void SetWindowName(const c8* _name, const c8* _title);


	void SetWindowSize(s32 _width, s32 _height,b8 _fullScreen);


	void ChangeWindowTitle(const c8* _title);


	b8 Create_Window(const HINSTANCE hInstance, const s32 nWindMode);


	LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


	b8 IsLoop();

	b8 CheckPeekMessage();


	HINSTANCE GetHinstance() { return m_hInstance; };

	HWND GetHWND() { return m_hWnd; };

	MSG GetMSG() { return m_msg; };

	s32 GetWindowWidth() { return m_width; };

	s32 GetWindowHeight() { return m_height; };

	CWindow() {};//コンストラクタ
	~CWindow() {};//デストラクタ

private:

	const c8* m_window_name = nullptr;//ウィンドウ名
	const c8* m_window_title = nullptr;//ウィンドウタイトル
	b8 m_full_screen_flag = false;//フルスクリーン設定

	HWND m_hWnd = nullptr;//ウィンドウハンドル
	MSG m_msg = {};//メッセージ
	s32 m_width = 0;//ウィンドウの横幅
	s32 m_height = 0;//ウィンドウの高さ
	HINSTANCE m_hInstance = nullptr;//ウィンドウ構造体


	HRESULT WNDC_initialize(const HINSTANCE hInstance,const c8* name);
	HRESULT HWND_initialize(const HINSTANCE hInstance, const c8* name, const c8* title,b8 full_screen);

};

EDENS_NAMESPACE_END

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


#endif