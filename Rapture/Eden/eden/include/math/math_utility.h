/////////////////////////////////////////////////////////////////////////////
///@file	math_utility.h
///@brief	CMathUtility define
///@data	2020/06/25
////////////////////////////////////////////////////////////////////////////
#ifndef __MATH_UTILITY_H__
#define __MATH_UTILITY_H__

EDENS_NAMESPACE_BEGIN

/*
*	@brief Math Utility Class
*/
class CMathUtility
{
public:

	/**
	 * @brief 値を範囲内に収める
	 * @tparam Number		数値型
	 * @param[in] target	対象の値
	 * @param[in] min		最小値
	 * @param[in] max		最大値
	 * @return 範囲内に収めた値
	 */
	template <typename Number>
	static Number Clamp(Number target, Number min, Number max);

	/**
	 * @brief 値を0～1の範囲内に収める
	 * @tparam Number		数値型
	 * @param[in] target	対象の値
	 * @return 範囲内に収めた値
	 */
	template <typename Number>
	static Number Saturate(Number target);

	/**
	 * @brief 値を循環シフトさせる
	 * @tparam Number		数値型
	 * @param[in] _Bit	シフトさせるビット数
	 * @param[in] _Target シフトさせる値
	 * @return シフトさせた値
	 */
	template <typename Number>
	static Number CircularShift(u8 _Bit, Number _Target);

	/**
	 * @brief 値が範囲内に収まっているかどうかを判定する
	 * @tparam Numer		数値型
	 * @param[in] _Target 対象の値
	 * @param[in] _A 範囲の左端
	 * @param[in] _B 範囲の右端
	 * @return 範囲内に収まっているかどうか
	 */
	template <typename Number>
	static bool CheckWithinRange(Number _Target, Number _A, Number _B);

	/**
	 * @brief 値を指定アラインに沿う形に調整する
	 * @tparam Integer	整数型
	 * @param[in] _Target 調整する値
	 * @param[in] _Align アライン
	 * @return 調整した値
	 * @details 256バイトアラインに調整する場合は、_Alignに0xFFを指定する
	 */
	template <typename Integer>
	static Integer AlignNum(Integer _Target, Integer _Align);

};


EDENS_NAMESPACE_END

#include "eden/include/math/math_utility.inl"

#endif