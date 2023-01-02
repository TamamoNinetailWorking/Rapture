#pragma once

//#include <memory>
//#include <d3d12.h>

#include <eden/include/Resource/Resource.h>
#include "ShaderPreDefine.h"
//#include <eden/include/Resource/ResourceInitializer.h>

//struct ID3DBlob;

EDENS_NAMESPACE_BEGIN

struct FResourceInitializerBase;

EDENS_NAMESPACE_END

ATLANTIS_NAMESPACE_BEGIN

struct FShaderBaseInitializer;
struct FShaderData;

class CShaderBase : public EDENS_NAMESPACE::CResource
{
public:

	virtual bool Initialize(const EDENS_NAMESPACE::FResourceInitializerBase* _Initializer);

	virtual void Finalize() override;

	PShaderCode GetShaderByteCode() const;
	uint64 GetBufferSize() const;


	CShaderBase();
	virtual ~CShaderBase();

protected:

	ObjectPtr(FShaderData) m_ShaderData = nullptr;

	bool CreateShader(const FShaderBaseInitializer* _Initializer);

	//bool CompileShaderFromFile(const FShaderBaseInitializer* _Initializer);

	//bool CompileShaderFromMemory(const FShaderBaseInitializer* _Initializer);

	//

	//
	////std::unique_ptr<ID3DBlob> m_ShaderBlob = nullptr;
	////ObjectPtr(ID3DBlob) m_ShaderBlob = nullptr;

	//ObjectPtr(ShaderCode) m_ShaderCode = nullptr;
	//uint64 m_BufferSize = 0;

	static Hash160& TargetSet(FShaderBaseInitializer* _Initializer);

};


ATLANTIS_NAMESPACE_END