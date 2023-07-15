#pragma once

#include "Object.h"

EDENS_NAMESPACE_BEGIN

/**
 * @brief オブジェクトが無効かどうかを判定する
 * 
 * \param[in] _Object 判定するオブジェクト
 * \return オブジェクトが無効かどうか
 */
inline bool IsInvalid(const CObject* _Object)
{
	if (!_Object) { return false; }
	return _Object->IsPendingKill();
}

/**
 * @brief オブジェクトが有効かどうかを判定する
 * 
 * \param[in] _Object 判定するオブジェクト
 * \return オブジェクトが有効かどうか
 */
inline bool Invalid(const CObject* _Object)
{
	return !IsInvalid(_Object);
}

EDENS_NAMESPACE_END