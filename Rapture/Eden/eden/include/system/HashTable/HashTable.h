/*****************************************************************//**
 * \file   HashTable.h
 * \brief  ハッシュテーブルの定義
 *********************************************************************/
#pragma once

#include <map>
#include <vector>
#include <string>

#include <eden/include/system/sha1/sha1.h>
#include <eden/include/template/singleton.h>

// 文字列とデータの塊のハッシュは別で持つかな・・・
// SHA1とGUIDとか、使い分けた方がデータ扱いやすいかもしれない

EDENS_NAMESPACE_BEGIN

/**
 * @brief ハッシュテーブルクラス
 * @note 現在のところSHA1で対応
 */
class CHashTable : public CSingleton<CHashTable>
{
public:

	using DataBlob = std::vector<uint8>;
	using String = std::string;

	/**
	 * @brief ハッシュ値をテーブルに設定する
	 * 
	 * \param[in] _Data ハッシュ化するデータ
	 */
	void SetValue(const DataBlob& _Data);

	/**
	 * @brief ハッシュ値をテーブルに設定する
	 * 
	 * \param[in] _string ハッシュ化する文字列
	 */
	void SetValue(const String& _string);

	/**
	 * @brief ハッシュ値をテーブルに設定する
	 * 
	 * \param[in] _string ハッシュ化する文字列
	 */
	void SetValue(const char* _string);

	/**
	 * @brief ハッシュ値を生成する
	 * 
	 * \param[in] _Data ハッシュ化するデータ
	 * \return ハッシュ値(SHA1)
	 */
	const Hash160 CreateHash(const DataBlob& _Data);

	/**
	 * @brief ハッシュ値を生成する
	 *
	 * \param[in] _string ハッシュ化する文字列
	 * \return ハッシュ値(SHA1)
	 * @note std::stringで受ける
	 */
	const Hash160 CreateHash(const String& _string);

	/**
	 * @brief ハッシュ値を生成する
	 *
	 * \param[in] _string ハッシュ化する文字列
	 * \return ハッシュ値(SHA1)
	 */
	const Hash160 CreateHash(const char* _string);

	/**
	 * @brief ハッシュ値を生成する
	 *
	 * \param[in] _string ハッシュ化する文字列
	 * \return ハッシュ値(SHA1)
	 */
	const Hash160 CreateHash(const uint8* _string);

	/**
	 * @brief ハッシュ値を取得する
	 * 
	 * \param[in] _Data ハッシュ化するデータ
	 * \return ハッシュ値(SHA1)
	 */
	const Hash160 GetHash(const DataBlob& _Data) const;

	/**
	 * @brief ハッシュ値を取得する
	 *
	 * \param[in] _string ハッシュ化する文字列
	 * \return ハッシュ値(SHA1)
	 */
	const Hash160 GetHash(const char* _string) const;

	/**
	 * @brief ハッシュ値を取得する
	 *
	 * \param[in] _string ハッシュ化する文字列
	 * \return ハッシュ値(SHA1)
	 */
	const Hash160 GetHash(const String& _string) const;

	/**
	 * @brief ハッシュ値を取得する
	 *
	 * \param[in] _string ハッシュ化する文字列
	 * \return ハッシュ値(SHA1)
	 */
	const Hash160 GetHash(const uint8* _string) const;

	/**
	 * @brief ハッシュ化したデータの元のデータをバイナリで取得する
	 * 
	 * \param[in] _Hash ハッシュ値
	 * \return ハッシュ化した元のデータ
	 */
	const uint8* GetHashData(const Hash160& _Hash) const;

	/**
	 * @brief ハッシュ化したデータの元のデータをバイナリで取得する
	 *
	 * \param[in] _Hash ハッシュ値
	 * \return ハッシュ化した元のデータ
	 */
	const char* GetHashValue(const Hash160& _hash) const;


private:

	typedef std::map<Hash160,String> StringHashTable;
	typedef std::map<Hash160,DataBlob> HashTable;
	
	/** 文字列のハッシュテーブル */
	StringHashTable m_StringHashTable = {};

	/** バイナリデータのハッシュテーブル */
	HashTable m_HashTable = {};
};


EDENS_NAMESPACE_END

#define CHash160(str) EDENS_NAMESPACE::CHashTable::GetInstance().CreateHash(str)
#define RHash160(hash) EDENS_NAMESPACE::CHashTable::GetInstance().GetHashValue(hash)
#define DHash160(hash) EDENS_NAMESPACE::CHashTable::GetInstance().GetHashData(hash)
static const Hash160 INVALID_HASH = Hash160();