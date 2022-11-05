#pragma once

#include <memory>

#include <d3d12.h>

#include <eden/include/Resource/Resource.h>

ATLANTIS_NAMESPACE_BEGIN

class CShaderBase : public EDENS_NAMESPACE::CResource
{
protected:

	struct FInitializerMiddle : public CResource::FInitializerBase
	{
		Hash160 FileNameHash = {};
		Hash160 FuncNameHash = {};
	protected:
		Hash160 TargetHash = {};

		friend class CShaderBase;
	};

public:

	typedef void* ShaderCode;


	virtual bool Initialize(const FInitializerBase* _Initializer);

	virtual void Finalize();

	ShaderCode GetShaderBytecod() const { return m_ShaderBlob->GetBufferPointer(); }
	uint64 GetBufferSize() const { return m_ShaderBlob->GetBufferSize(); };


	CShaderBase() {};
	virtual ~CShaderBase();

protected:

	bool CompileShaderFromFile(const FInitializerMiddle* _Initializer);

	
	std::unique_ptr<ID3DBlob> m_ShaderBlob = nullptr;

	static Hash160& TargetSet(FInitializerMiddle* _Initializer);

};


ATLANTIS_NAMESPACE_END