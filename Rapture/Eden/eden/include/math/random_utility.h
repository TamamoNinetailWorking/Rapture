/////////////////////////////////////////////////////////////////////////////
///@file	random_utility.h
///@brief	CRandomizer define
///@data	2020/06/25
////////////////////////////////////////////////////////////////////////////
#ifndef __RANDOM_UTILITY_H__
#define __RANDOM_UTILITY_H__

//Include Header
#include <math.h>
#include <random>
#include <eden/include/template/singleton.h>

EDENS_NAMESPACE_BEGIN

/*
*	Random Number Creator
*/
class CRandomizer : public CSingleton<CRandomizer>
{
protected:
	
	friend class CSingleton<CRandomizer>;

public:

	template <typename Integer>
	Integer RandomRangeInteger(Integer min, Integer max);

	template <typename Real>
	Real RandomRangeReal(Real min, Real max);

protected:

	CRandomizer();

	std::random_device m_rnd;
	std::mt19937 m_mt;
};

EDENS_NAMESPACE_END

#include "eden/include/math/random_uitility.inl"

#endif