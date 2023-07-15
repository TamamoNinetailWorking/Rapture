/*****************************************************************//**
 * \file   FileLoader.h
 * \brief  ファイルロードツールの定義
 *********************************************************************/
#pragma once

#include <vector>

EDENS_NAMESPACE_BEGIN

/**
 * .ファイルロードクラス
 */
class CFileLoader
{
public:

	/**
	 * @brief ファイル読み込み
	 * 
	 * \param[in] _FileName ファイル名
	 * \return ファイル読み込みに成功したかどうか
	 */
	bool FileLoad(const char* _FileName);

	/**
	 * @brief ファイル読み込み
	 * 
	 * \param[in] _FileNameHash ファイル名のハッシュ値
	 * \return ファイル読み込みに成功したかどうか
	 */
	bool FileLoad(Hash160 _FileNameHash);

	/**
	 * @brief 保持データのリセット
	 * @details ファイルロードによって保持しているデータをリセットする
	 */
	void ResetData();

	/**
	 * @brief ファイル名のハッシュ値取得
	 */
	const Hash160& GetFileName() const { return m_FileName; };
	
	/**
	 * @brief ファイルデータ取得
	 */
	const uint8* GetData() const { return m_Data.data(); };
	
	/**
	 * @brief ファイルデータのサイズ取得
	 */
	uint32 GetSize() const { return m_DataSize; };

	/**
	 * @brief コンストラクタ
	 * 
	 */
	CFileLoader();

	/**
	 * @brief デストラクタ
	 * 
	 */
	~CFileLoader();

private:

	/** ファイル名のハッシュ値 */
	Hash160 m_FileName = {};

	//ObjectPtr(uint8) m_Data = nullptr;

	/** ファイルデータ */
	std::vector<uint8> m_Data = {};

	/** ファイルデータのサイズ */
	uint32 m_DataSize = 0;

};



EDENS_NAMESPACE_END