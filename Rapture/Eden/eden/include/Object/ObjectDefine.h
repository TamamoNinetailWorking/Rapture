#pragma once

#include "Object.h"

EDENS_NAMESPACE_BEGIN

inline bool IsInvalid(const CObject* _Object)
{
	if (!_Object) { return false; }
	return _Object->IsPendingKill();
}

inline bool Invalid(const CObject* _Object)
{
	return !IsInvalid(_Object);
}

EDENS_NAMESPACE_END