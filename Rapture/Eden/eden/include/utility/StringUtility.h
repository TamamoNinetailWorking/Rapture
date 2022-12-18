#pragma once


EDENS_NAMESPACE_BEGIN

namespace StringUtility
{
	s32 ToWString(WCHAR* _Wstring, u32 _size, const char* _String, size_t& _Length);
};

EDENS_NAMESPACE_END