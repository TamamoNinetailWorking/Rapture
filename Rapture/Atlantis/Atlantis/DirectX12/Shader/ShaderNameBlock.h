#pragma once

#include "ShaderName.h"

ATLANTIS_NAMESPACE_BEGIN

struct FShaderNameBlock
{
	FShaderName VS = {};
	FShaderName HS = {};
	FShaderName DS = {};
	FShaderName GS = {};
	FShaderName PS = {};
};

ATLANTIS_NAMESPACE_END