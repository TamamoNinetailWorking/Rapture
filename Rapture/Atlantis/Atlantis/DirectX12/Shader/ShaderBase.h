#pragma once

//#include <memory>
#include <d3d12.h>

#include <eden/include/Resource/Resource.h>
//#include <eden/include/Resource/ResourceInitializer.h>

//struct ID3DBlob;

EDENS_NAMESPACE_BEGIN

struct FResourceInitializerBase;

EDENS_NAMESPACE_END

ATLANTIS_NAMESPACE_BEGIN

struct FShaderBaseInitializer;

class CShaderBase : public EDENS_NAMESPACE::CResource
{
public:

	typedef void* ShaderCode;


	virtual bool Initialize(const EDENS_NAMESPACE::FResourceInitializerBase* _Initializer);

	virtual void Finalize() override;

	ShaderCode GetShaderBytecod() const;
	uint64 GetBufferSize() const;


	CShaderBase() {};
	virtual ~CShaderBase();

protected:

	bool CompileShaderFromFile(const FShaderBaseInitializer* _Initializer);

	
	//std::unique_ptr<ID3DBlob> m_ShaderBlob = nullptr;
	ObjectPtr(ID3DBlob) m_ShaderBlob = nullptr;

	static Hash160& TargetSet(FShaderBaseInitializer* _Initializer);

};


ATLANTIS_NAMESPACE_END