/*****************************************************************//**
 * \file   Object.h
 * \brief  管理データの基底オブジェクトクラス定義
 *********************************************************************/
#pragma once

EDENS_NAMESPACE_BEGIN

/**
 * @brief 管理データの基底オブジェクトクラス
 */
class CObject
{
public:

	/**
	 * @brief 削除予定かどうか
	 * 
	 * \return 削除予定かどうか
	 */
	bool IsPendingKill() const;

	/**
	 * @brief 削除予定フラグを設定
	 * 
	 * \param[in] _IsKill 削除予定かどうか
	 */
	void SetPendingKill(bool _IsKill);

private:

	/** 削除予定かどうか */
	bool m_IsPendingKill = false;

};

EDENS_NAMESPACE_END