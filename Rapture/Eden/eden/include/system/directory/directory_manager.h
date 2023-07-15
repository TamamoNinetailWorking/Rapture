/////////////////////////////////////////////////////////////////////////////
///@file	directory_manager.h
///@brief	directory_manager	define
///@data	2020/06/25
////////////////////////////////////////////////////////////////////////////
#ifndef __DIRECTORY_MANAGER_H__
#define __DIRECTORY_MANAGER_H__

//Include Header
#include <eden/include/template/singleton.h>

EDENS_NAMESPACE_BEGIN

/*
*	@brief Pointer Directory Path
*/
class CDirectoryManager : public CSingleton<CDirectoryManager>
{
protected:

	friend class CSingleton<CDirectoryManager>;

public:

	/**
	 * @brief デストラクタ
	 */
	~CDirectoryManager() { Exit(); };

	/**
	 * @brief 初期化処理
	 * 
	 * \param[in] name_size　ディレクトリ名のサイズ
	 */
	void Init(u32 name_size = 1024);

	/**
	 * @brief 終了処理
	 * 	 * 
	 * 	 
	 */
	void Exit();

	/**
	 * @brief デフォルトのディレクトリに設定
	 * 
	 */
	void RestoreDefaultDirectory();

	/**
	 * @brief 参照するディレクトリ階層を設定
	 * 
	 * \param[in] directory 参照するディレクトリ階層のパス
	 */
	void SetDirectory(const c8* directory);

protected:

	/** デフォルトのディレクトリパス */
	c8* m_default_current_directory = nullptr;

	/** ディレクトリ名のサイズ */
	u32 m_directory_name_size = 0;

};

EDENS_NAMESPACE_END

#endif