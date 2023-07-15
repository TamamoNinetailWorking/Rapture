/*****************************************************************//**
 * \file   StringUtility.h
 * \brief  文字列操作のユーティリティ定義
 *********************************************************************/
#pragma once


EDENS_NAMESPACE_BEGIN

namespace StringUtility
{
	using String = std::string;

	/**
	 * @brief ワイド文字列に変換
	 * @param[out] _Wstring 変換後のワイド文字列
	 * @param[in] _size 変換後のワイド文字列のサイズ
	 * @param[in] _String 変換前の文字列
	 * @param[in] _Length 変換前の文字列の長さ
	 * @return 変換に成功したかどうか
	 */
	s32 ToWString(WCHAR* _Wstring, u32 _size, const char* _String, size_t& _Length);

	/**
	 * @brief エラーをPRINT表示
	 * @param[in] エラーコマンド
	 */
	void ErrorPrint(errno_t _Error);

	/**
	 * @brief　文字列に指定の文字が含まれているかどうか
	 * @param[in] _Text 文字列
	 * @param[in] _Splitter 指定の文字
	 * @return 含まれているかどうか
	 */
	bool ExitChara(const String& _Text, const char& _Splitter);

	/**
	 * @brief 文字列を結合する
	 * @param[in] _First 一つ目の文字列
	 * @param[in] _Second 二つ目の文字列
	 * @param[in] _Splitter 結合する際に挟む文字
	 * @return 結合した文字列
	 */
	const String CombineText(const char* _First, const char* _Second, const char& _Splitter = 0x00);

	/**
	 * @brief 文字列を結合する
	 * @param[in] _First 一つ目の文字列
	 * @param[in] _Second 二つ目の文字列
	 * @param[in] _Splitter 結合する際に挟む文字
	 * @return 結合した文字列
	 */
	const String CombineText(const String& _First, const String& _Second,const char& _Splitter= 0x00);

	using StringPair = std::pair<String, String>;

	/**
	 * @brief 文字列を分割する
	 * @param[in] _Text 分割する文字列
	 * @param[in] _Splitter 分割する際に使う文字
	 * @return 分割した文字列のペア
	 */
	const StringPair SeparateText(const String& _Text, const char& _Splitter);

};

EDENS_NAMESPACE_END