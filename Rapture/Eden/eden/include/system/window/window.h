/////////////////////////////////////////////////////////////////////////////
///@file	window.h
///@brief	CWindow define
///@data	2020/06/26
////////////////////////////////////////////////////////////////////////////
#ifndef __WINDOW_H__
#define __WINDOW_H__

EDENS_NAMESPACE_BEGIN

/*
*	@brief GUIウィンドウ生成クラス
*/
class CWindow
{
public:

	/**
	 * @brief ウィンドウ初期化処理
	 * 
	 * \param[in] _Initializer ウィンドウ初期化構造体
	 * \return 生成に成功したかどうか
	 */
	b8 Initialize(const struct FWindowInitializer* _Initializer);

	/**
	 * @brief ウィンドウ終了処理
	 * 
	 */
	void Finalize();

	/**
	 * @brief ウィンドウの表示
	 * 
	 * \param[in] nWindMode ウィンドウの表示モード
	 */
	void EdenShowWindow(const s32 nWindMode) const;

	/**
	 * @brief ウィンドウのタイトル変更
	 * 
	 * \param[in] _title　変更するタイトル
	 */
	void ChangeWindowTitle(const c8* _title);

	/**
	 * @brief ウィンドウのメッセージ処理
	 * 
	 * \param[in] hWnd　ウィンドウハンドル
	 * \param[in] uMsg　メッセージ
	 * \param[in] wParam　メッセージの追加情報
	 * \param[in] lParam　メッセージの追加情報
	 * \return メッセージ処理の結果
	 */
	LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**
	 * @brief ウィンドウのメッセージ処理
	 * 
	 * \return メッセージ処理の結果
	 */
	b8 IsMessageQuit();

	/**
	 * @brief ウィンドウのメッセージ処理
	 * 
	 * \return OSのメッセージがないかどうか
	 */
	b8 CheckPeekMessage();

	/**
	 * @brief ウィンドウインスタンスの取得
	 * @return ウィンドウインスタンス
	 */
	HINSTANCE GetHinstance() const { return m_hInstance; };

	/**
	 * @brief ウィンドウハンドルの取得
	 * @return ウィンドウハンドル
	 */
	HWND GetHWND() const { return m_hWnd; };

	/**
	 * @brief ウィンドウのメッセージの取得
	 * @return ウィンドウのメッセージ
	 */
	MSG GetMSG() const { return m_msg; };

	/**
	 * @brief ウィンドウの横幅の取得
	 * @return	ウィンドウの横幅
	 */
	s32 GetWindowWidth() const { return m_width; };

	/**
	 * @brief ウィンドウの高さの取得
	 * @return ウィンドウの高さ
	 */
	s32 GetWindowHeight() const { return m_height; };

	/**
	 * @brief コンストラクタ
	 */
	CWindow() {};

	/**
	 * @brief デストラクタ
	 */
	~CWindow() {};

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

	/**
	 * @brief ウィンドウ記述子の初期化
	 * 
	 * \param[in] hInstance ウィンドウインスタンス
	 * \param[in] name ウィンドウ名
	 * \return 成功したかどうか
	 */
	HRESULT WNDC_initialize(const HINSTANCE hInstance,const c8* name);

	/**
	 * @brief ウィンドウハンドルの初期化
	 * 
	 * \param[in] hInstance ウィンドウインスタンス
	 * \param[in] name ウィンドウ名
	 * \param[in] title ウィンドウタイトル
	 * \param[in] full_screen フルスクリーン設定
	 * \return 成功したかどうか
	 */
	HRESULT HWND_initialize(const HINSTANCE hInstance, const c8* name, const c8* title,b8 full_screen);

	/**
	 * @brief ウィンドウの名前、タイトルの設定
	 * 
	 * \param[in] _name ウィンドウの名前
	 * \param[in] _title ウィンドウのタイトル
	 */
	void SetWindowName(const c8* _name, const c8* _title);

	/**
	 * @brief ウィンドウのサイズの設定
	 * 
	 * \param[in] _width ウィンドウの横幅
	 * \param[in] _height ウィンドウの高さ
	 * \param[in] _fullScreen フルスクリーン設定
	 */
	void SetWindowSize(s32 _width, s32 _height, b8 _fullScreen);

	/**
	 * @brief ウィンドウの作成処理
	 * 
	 * \param[in] hInstance ウィンドウインスタンス
	 * \return 成功したかどうか
	 */
	b8 EdenCreateWindow(const HINSTANCE hInstance/*, const s32 nWindMode*/);

};

EDENS_NAMESPACE_END

/**
 * @brief ウィンドウのメッセージ処理
 *
 * \param[in] hWnd　ウィンドウハンドル
 * \param[in] uMsg　メッセージ
 * \param[in] wParam　メッセージの追加情報
 * \param[in] lParam　メッセージの追加情報
 * \return メッセージ処理の結果
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


#endif

