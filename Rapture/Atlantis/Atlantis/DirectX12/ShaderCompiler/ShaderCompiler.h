#pragma once

ATLANTIS_NAMESPACE_BEGIN

struct FShaderData;

class CShaderCompiler
{
public:

	struct FInitializer
	{
		Hash160 FileAndFuncHash = {};
		Hash160 TargetHash = {};
		const void* Data = nullptr;
		uint32 Size = 0;
	};

	bool ShaderCompile(FShaderData* _DestData,const FInitializer& _Initializer) ;

	CShaderCompiler();
	~CShaderCompiler();

private:

	class Impl;
	ObjectPtr(Impl) m_Impl = nullptr;

};

ATLANTIS_NAMESPACE_END