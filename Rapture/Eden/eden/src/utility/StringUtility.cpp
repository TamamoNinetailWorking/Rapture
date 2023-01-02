#include "eden/include/utility/StringUtility.h"
#include <locale.h>

EDENS_NAMESPACE_USING;

using String = StringUtility::String;
using StringPair = StringUtility::StringPair;

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
	char error[94] = {};
	strerror_s(error,_Error);
	PRINT("%s\n", error);
}

bool StringUtility::ExitChara(const String& _Text, const char& _Splitter)
{
	return std::count(_Text.begin(), _Text.end(), _Splitter) > 0;
}

const String StringUtility::CombineText(const char* _First, const char* _Second, const char& _Splitter)
{
	String first(_First);
	String second(_Second);
	return CombineText(first,second,_Splitter);
}

const String StringUtility::CombineText(const String& _First, const String& _Second, const char& _Splitter)
{
	if (_First.empty()) { return String(); }
	String ret = String();
	ret = _First + _Splitter;
	ret = ret + _Second;

	return ret;
}

const StringPair StringUtility::SeparateText(const String& _Text, const char& _Splitter)
{
	s32 index = SCast<s32>(_Text.find(_Splitter));
	StringPair ret = {};
	ret.first = _Text.substr(0, index);
	ret.second = _Text.substr(index + 1, _Text.length() - index - 1);
	return ret;
}
