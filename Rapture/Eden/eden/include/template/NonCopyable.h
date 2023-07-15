/*****************************************************************//**
 * \file   NonCopyable.h
 * \brief  コピー禁止クラス定義
 *********************************************************************/
#pragma once

//派生クラスをコピー禁止にするために
//代入演算子のオーバーロードをprivate化
//	→Singletonの基底クラスだった。

/**
 * @brief コピー禁止クラスの基底クラス
 */
class CNonCopyable
{
protected://継承しても引き続き使用可能

	/**
	 * @brief コンストラクタ
	 */
	CNonCopyable() {};

	/**
	 * @brief デストラクタ
	 */
	virtual ~CNonCopyable() {};

private:
	//代入処理に当たるものをprivateにして使用禁止化

	/**
	 * @brief コピーコンストラクタ
	 * 
	 * @details コピー禁止のためにprivate化 
	 */
	CNonCopyable(const CNonCopyable&);

	/**
	 * @brief 代入演算子
	 * 
	 * @details コピー禁止のためにprivate化
	 */
	const CNonCopyable& operator=(const CNonCopyable&);
};