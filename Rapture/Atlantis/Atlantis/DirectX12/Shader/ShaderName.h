#pragma once

ATLANTIS_NAMESPACE_BEGIN

struct FShaderName
{
	using String =  std::string;
public:

	String File = {};
	String Function = {};

public:

	const Hash160 GetShaderName(const char& _Splitter = '*') const;

};

ATLANTIS_NAMESPACE_END