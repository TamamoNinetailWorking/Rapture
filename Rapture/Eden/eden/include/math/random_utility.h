/////////////////////////////////////////////////////////////////////////////
///@file	random_utility.h
///@brief	CRandomizer define
////////////////////////////////////////////////////////////////////////////
#ifndef __RANDOM_UTILITY_H__
#define __RANDOM_UTILITY_H__

//Include Header
#include <math.h>
#include <random>
#include <eden/include/template/singleton.h>

EDENS_NAMESPACE_BEGIN

/*
*	@brief Random Number Creator
*/
class CRandomizer : public CSingleton<CRandomizer>
{
protected:
	
	friend class CSingleton<CRandomizer>;

public:

	/**
	 * @brief 整数型の乱数を生成
	 * @tparam Integer 整数型
	 * @param[in] min 乱数の最小値
	 * @param[in] max 乱数の最大値
	 * @return 生成された乱数
	 */
	template <typename Integer>
	Integer RandomRangeInteger(Integer min, Integer max);

	/**
	 * @brief 実数型の乱数を生成
	 * @tparam Real 実数型
	 * @param[in] min 乱数の最小値
	 * @param[in] max 乱数の最大値
	 * @return 生成された乱数
	 */
	template <typename Real>
	Real RandomRangeReal(Real min, Real max);

protected:

	/**
	 * @brief コンストラクタ
	 * 
	 */
	CRandomizer();

	/** 乱数機 */
	std::random_device m_rnd;

	/**  メルセンヌ・ツイスターを利用した乱数機 */
	std::mt19937 m_mt;
};

EDENS_NAMESPACE_END

#include "eden/include/math/random_uitility.inl"

#endif