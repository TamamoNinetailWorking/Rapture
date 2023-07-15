#include "ShaderName.h"

#include <eden/include/utility/StringUtility.h>

USING_ATLANTIS;
EDENS_NAMESPACE_USING;

const Hash160 ATLANTIS_NAMESPACE::FShaderName::GetShaderName(const char& _Splitter) const
{
	Hash160 id = Hash160();

	if (File.empty() && Function.empty())
	{
		return id;
	}

	id = CHash160(StringUtility::CombineText(File, Function, _Splitter));
	return id;
}
