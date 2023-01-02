#pragma once

#include "ShaderPreDefine.h"

//struct ID3DBlob;

ATLANTIS_NAMESPACE_BEGIN

struct FShaderData
{
	ObjectPtr(ShaderCode) m_ShaderCode = nullptr;
	uint64 m_BufferSize = 0;
	//ObjectPtr(ID3DBlob) m_ShaderBlob = nullptr;
};

ATLANTIS_NAMESPACE_END