#include "DirectXUtility.h"

#include <locale.h>

USING_ATLANTIS;

int32 Utility::ToWString(WCHAR* _Wstring, uint32 _size,const char* _String, size_t& _Length)
{
	int32 error = 0;
	setlocale(LC_ALL, "japanese");
	error = mbstowcs_s(&_Length, _Wstring, SCast<size_t>(_size),_String, _TRUNCATE);
	return error;
}
