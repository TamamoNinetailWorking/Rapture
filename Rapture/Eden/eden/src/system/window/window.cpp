/////////////////////////////////////////////////////////////////////////////
///@file	window.cpp
///@brief	CWindow implementation
///@data	2020/06/26
////////////////////////////////////////////////////////////////////////////

//Include Header
#include <eden/include/system/window/window.h>
#include <eden/include/system/window/window_def.h>

EDENS_NAMESPACE_BEGIN


b8 CWindow::Initialize(const FWindowInitializer* _Initializer)
{
	if (_Initializer == nullptr) { return false; }

	SetWindowName(_Initializer->WindowName, _Initializer->WindowTitle);
	SetWindowSize(_Initializer->WindowWidth, _Initializer->WindowHeight,_Initializer->IsFullScreen);

	b8 result = EdenCreateWindow(_Initializer->Instance/*, _Initializer->WindowMode*/);

	return result;
}

void CWindow::Finalize()
{
	UnregisterClass(m_ApplicationClass.lpszClassName, m_ApplicationClass.hInstance);
}

void CWindow::SetWindowName(const c8* _name, const c8* _title)
{
	m_window_name = _name;
	m_window_title = _title;
}


void CWindow::SetWindowSize(s32 _width, s32 _height, b8 _fullScreen)
{
	m_width = _width;
	m_height = _height;
	m_full_screen_flag = _fullScreen;
}


void CWindow::EdenShowWindow(const s32 nWindMode) const
{
	ShowWindow(m_hWnd, nWindMode);
	//UpdateWindow(m_hWnd);
}

void CWindow::ChangeWindowTitle(const c8* _title)
{
	m_window_title = _title;
	SetWindowText(m_hWnd, _title);
}


HRESULT CWindow::WNDC_initialize(const HINSTANCE hInstance,const c8* name) 
{
	WNDCLASSEX wcex;
	wcex.hInstance = hInstance;//インスタンス値のセット
	wcex.lpszClassName = name;//クラス名
	wcex.lpfnWndProc = (WNDPROC)WndProc;//ウィンドウメッセージ
	wcex.style = 0;//ウィンドウスタイル
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hIcon = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);//ラージアイコン
	wcex.hIconSm = LoadIcon((HINSTANCE)NULL, IDI_WINLOGO);//スモールアイコン
	wcex.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);//カーソルスタイル
	wcex.lpszMenuName = 0;//メニューなし
	wcex.cbClsExtra = 0;//エキストラなし
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);//背景色白

	if (!RegisterClassEx(&wcex)) 
	{
		return S_FALSE;
	}
	else
	{
		m_ApplicationClass = wcex;
		return S_OK;
	}
}


HRESULT CWindow::HWND_initialize(const HINSTANCE hInstance, const c8* name, const c8* title,b8 full_screen)
{
	if (full_screen){//フルスクリーン
		m_hWnd = CreateWindow(
			name,//ウィンドウクラスの名前
			title,//タイトル
			WS_VISIBLE | WS_POPUP,//ウィンドウスタイル
			0, 0,//ウィンドウ位置 縦、横
			m_width, m_height,//ウィンドウサイズ
			NULL,//親ウィンドウなし
			(HMENU)NULL,//メニューなし
			hInstance,//インスタンスハンドル
			(LPVOID)NULL//追加引数なし
		);
	}//フルスクリーンEnd
	else 
	{//ウィンドウモード
		RECT rWindow, rClient;
		int width = m_width, height = m_height;

		m_hWnd = CreateWindow(
			name,//ウィンドウクラスの名前
			title,//タイトル
			WS_CAPTION | WS_SYSMENU,//ウィンドウスタイル
			0, 0,//ウィンドウ位置 縦、横（あとで中央に移動させる
			m_width, m_height,//ウィンドウサイズ
			HWND_DESKTOP,//親ウィンドウなし
			(HMENU)NULL,//メニューなし
			hInstance,//インスタンづはんどる
			(LPVOID)NULL//追加引数なし
		);

		//ウィンドウサイズを再計算(Metricsだけではフレームデザインとクライアント領域サイズが変わってしまう）
		GetWindowRect(m_hWnd, &rWindow);
		GetClientRect(m_hWnd, &rClient);
		width = (rWindow.right - rWindow.left) - (rClient.right - rClient.left) + m_width;
		height = (rWindow.bottom - rWindow.top) - (rClient.bottom - rClient.top) + m_height;
		SetWindowPos(
			m_hWnd,
			NULL,
			GetSystemMetrics(SM_CXSCREEN)/2-width/2,
			GetSystemMetrics(SM_CYSCREEN)/2-height/2,
			width-1,
			height-1,
			SWP_NOZORDER
		);
	}//ウィンドウモードEnd

	if (!m_hWnd) {
		return S_FALSE;
	}else {
		return S_OK;
	}
}

b8 CWindow::EdenCreateWindow(const HINSTANCE hInstance/*, const s32 nWindMode*/)
{
	m_hInstance = hInstance;

	//ウィンドウクラスの登録
	if (FAILED(WNDC_initialize(hInstance, m_window_name)))
	{
		MessageBox(m_hWnd, "ERROR!", "WNDC Error", MB_OK);
		return false;
	}

	//ウィンドウハンドルの登録
	if (FAILED(HWND_initialize(hInstance, m_window_name, m_window_title,m_full_screen_flag)))
	{
		MessageBox(m_hWnd, "ERROR!", "HWND Error", MB_OK);
		return false;
	}

	return true;
}

bool CWindow::IsMessageQuit()
{
	return m_msg.message== WM_QUIT;
}

bool CWindow::CheckPeekMessage()
{
	if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);

		return true;
	}
	
	return false;
}

LRESULT CWindow::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			//PostQuitMessage(0);
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

EDENS_NAMESPACE_END