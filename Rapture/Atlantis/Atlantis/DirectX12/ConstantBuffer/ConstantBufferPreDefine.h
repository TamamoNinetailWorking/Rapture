#pragma once

#include <eden/include/math/math_utility.h>

ATLANTIS_NAMESPACE_BEGIN

uint32 AlignBufferSize(uint32 _SrcSize)
{
	return EDENS_NAMESPACE::CMathUtility::AlignNum(_SrcSize, 0xffu);
}

ATLANTIS_NAMESPACE_END