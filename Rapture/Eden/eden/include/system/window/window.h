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


	b8 Initialize(const struct FWindowInitializer* _Initializer);

	void Finalize();


	void ChangeWindowTitle(const c8* _title);


	LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


	b8 IsMessageQuit();

	b8 CheckPeekMessage();


	HINSTANCE GetHinstance() const { return m_hInstance; };

	HWND GetHWND() const { return m_hWnd; };

	MSG GetMSG() const { return m_msg; };

	s32 GetWindowWidth() const { return m_width; };

	s32 GetWindowHeight() const { return m_height; };

	CWindow() {};//コンストラクタ
	~CWindow() {};//デストラクタ

private:

	const c8* m_window_name = nullptr;//ウィンドウ名
	const c8* m_window_title = nullptr;//ウィンドウタイトル
	b8 m_full_screen_flag = false;//フルスクリーン設定

	WNDCLASSEX m_ApplicationClass = {}; // アプリケーションクラス
	HINSTANCE m_hInstance = nullptr;//ウィンドウ構造体
	HWND m_hWnd = nullptr;//ウィンドウハンドル
	MSG m_msg = {};//メッセージ
	s32 m_width = 0;//ウィンドウの横幅
	s32 m_height = 0;//ウィンドウの高さ


	HRESULT WNDC_initialize(const HINSTANCE hInstance,const c8* name);
	HRESULT HWND_initialize(const HINSTANCE hInstance, const c8* name, const c8* title,b8 full_screen);

	void SetWindowName(const c8* _name, const c8* _title);
	void SetWindowSize(s32 _width, s32 _height, b8 _fullScreen);

	b8 EdenCreateWindow(const HINSTANCE hInstance, const s32 nWindMode);

};

EDENS_NAMESPACE_END

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


#endif