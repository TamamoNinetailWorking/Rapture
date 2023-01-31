#include "ConstantBufferPreDefine.h"

#include <eden/include/math/math_utility.h>

USING_ATLANTIS;


uint32 ATLANTIS_NAMESPACE::AlignBufferSize(uint32 _SrcSize)
{
	return EDENS_NAMESPACE::CMathUtility::AlignNum(_SrcSize, 0xffu);
}

