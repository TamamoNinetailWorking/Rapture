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

	template <typename Number>
	static Number CircularShift(u8 _Bit, Number _Target);

	template <typename Number>
	static bool CheckWithinRange(Number _Target, Number _A, Number _B);

};


EDENS_NAMESPACE_END

#include "eden/include/math/math_utility.inl"

#endif