#include "eden/include/utility/StringUtility.h"
#include <locale.h>

EDENS_NAMESPACE_USING;



s32 StringUtility::ToWString(WCHAR* _Wstring, u32 _size, const char* _String, size_t& _Length)
{
	s32 error = 0;
	setlocale(LC_ALL, "japanese");
	error = mbstowcs_s(&_Length, _Wstring, SCast<size_t>(_size), _String, _TRUNCATE);
	return error;
}

void StringUtility::ErrorPrint(errno_t _Error)
{
	if (_Error == 0) { return; }
	char error[256] = {};
	strerror_s(error,_Error);
	PRINT("%s\n", error);
}
