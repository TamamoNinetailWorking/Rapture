/////////////////////////////////////////////////////////////////////////////
///@file	math_utility.h
///@brief	CMathUtility define
///@data	2020/06/25
////////////////////////////////////////////////////////////////////////////
#ifndef __MATH_UTILITY_H__
#define __MATH_UTILITY_H__

EDENS_NAMESPACE_BEGIN

/*
*	Math Utility Class
*/
class CMathUtility
{
public:

	template <typename Number>
	static Number Clamp(Number target, Number min, Number max);

	template <typename Number>
	static Number Saturate(Number target);

};


EDENS_NAMESPACE_END

#include "eden/include/math/math_utility.inl"

#endif