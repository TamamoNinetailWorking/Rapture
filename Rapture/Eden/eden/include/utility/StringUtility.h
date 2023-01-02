#pragma once


EDENS_NAMESPACE_BEGIN

namespace StringUtility
{
	using String = std::string;

	s32 ToWString(WCHAR* _Wstring, u32 _size, const char* _String, size_t& _Length);

	void ErrorPrint(errno_t _Error);

	bool ExitChara(const String& _Text, const char& _Splitter);

	const String CombineText(const char* _First, const char* _Second, const char& _Splitter = 0x00);
	const String CombineText(const String& _First, const String& _Second,const char& _Splitter= 0x00);

	using StringPair = std::pair<String, String>;
	const StringPair SeparateText(const String& _Text, const char& _Splitter);

};

EDENS_NAMESPACE_END