/////////////////////////////////////////////////////////////////////////////
///@file	math_utility.inl
///@brief	CMathUtility implementation
///@data	2020/06/25
////////////////////////////////////////////////////////////////////////////
#ifndef __MATH_UTILITY_INL__
#define __MATH_UTILITY_INL__

EDENS_NAMESPACE_BEGIN

template <typename Number>
Number CMathUtility::Clamp(Number target, Number min, Number max)
{
	Number returner = target;
	if (target > max)
	{
		returner = max;
	}

	if (target < min)
	{
		returner = min;
	}

	return returner;
}

template <typename Number>
Number CMathUtility::Saturate(Number target)
{
	Clamp(target, 0, 1);
}

EDENS_NAMESPACE_END

#endif