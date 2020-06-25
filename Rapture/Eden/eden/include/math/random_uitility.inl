/////////////////////////////////////////////////////////////////////////////
///@file	random_utility.inl
///@brief	CRandomizer implementation
///@data	2020/06/25
////////////////////////////////////////////////////////////////////////////
#ifndef __RANDOM_UTILITY_INL__
#define __RANDOM_UTILITY_INL__

EDENS_NAMESPACE_BEGIN


CRandomizer::CRandomizer() : CSingleton<CRandomizer>()
{
	m_mt = std::mt19937(m_rnd());
}


template <typename Integer>
Integer CRandomizer::RandomRangeInteger(Integer min, Integer max)
{
	std::uniform_int_distribution<Integer> dst = std::uniform_int_distribution<Integer>(min, max);
	return dst(m_mt);
}

template <typename Real>
Real CRandomizer::RandomRangeReal(Real min, Real max)
{
	std::uniform_real_distribution<Real> dst = std::uniform_real_distribution<Real>(min, max);
	return dst(m_mt);
}

EDENS_NAMESPACE_END

#endif