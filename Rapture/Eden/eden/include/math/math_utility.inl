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

template <typename Number>
Number CMathUtility::CircularShift(u8 _Bit, Number _Target)
{
	u8 size = sizeof(_Target) * 8;//8 = BitNum
	return (_Target << _Bit) | (_Target >> (size - _Bit));
}

template <typename Number>
static bool CMathUtility::CheckWithinRange(Number _Target, Number _A, Number _B)
{
	return _A <= _Target && _Target < _B;
}

EDENS_NAMESPACE_END

#endif