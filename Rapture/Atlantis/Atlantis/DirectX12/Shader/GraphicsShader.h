#pragma once

#include "ShaderBase.h"

#include <d3d12.h>
#include <vector>
#include <d3dcompiler.h>

ATLANTIS_NAMESPACE_BEGIN

struct FVertexShaderInitializer;

class CGraphicsShader : public CShaderBase
{
public:

	~CGraphicsShader() override;
};

class CVertexShader : public CGraphicsShader
{
public:

	bool Initialize(const EDENS_NAMESPACE::FResourceInitializerBase* _Initializer) override;
	void Finalize() override;

	~CVertexShader() override;


	// こここそImplかな
	const D3D12_INPUT_ELEMENT_DESC* GetInputLayout() const { return m_InputLayout->data(); };
	uint32 GetInputNum() const { return SCast<uint32>(m_InputLayout->size()); };


private:

	typedef std::vector<D3D12_INPUT_ELEMENT_DESC> InputLayout;

	bool CreateReflection(const FVertexShaderInitializer* _Initializer);
	bool CreateInputLayout(const FVertexShaderInitializer* _Initializer);

	//std::unique_ptr<InputLayout> m_InputLayout = nullptr;
	ObjectPtr(InputLayout) m_InputLayout = nullptr;
	//std::unique_ptr<ID3D12ShaderReflection> m_Reflection = nullptr;
	ObjectPtr(ID3D12ShaderReflection) m_Reflection = nullptr;
};

class CHullShader : public CGraphicsShader
{
public:

	bool Initialize(const EDENS_NAMESPACE::FResourceInitializerBase* _Initializer) override;

};

class CDomainShader : public CGraphicsShader
{
public:

	bool Initialize(const EDENS_NAMESPACE::FResourceInitializerBase* _Initializer) override;

};

class CGeometryShader : public CGraphicsShader
{
public:

	bool Initialize(const EDENS_NAMESPACE::FResourceInitializerBase* _Initializer) override;

};

class CPixelShader : public CGraphicsShader
{
public:

	bool Initialize(const EDENS_NAMESPACE::FResourceInitializerBase* _Initializer) override;


};

ATLANTIS_NAMESPACE_END