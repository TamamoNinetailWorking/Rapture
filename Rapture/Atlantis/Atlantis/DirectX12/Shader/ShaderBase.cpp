#include "ShaderBase.h"
#include "ShaderBaseInitializer.h"
#include "ShaderData.h"

#include <d3d12.h>
#include <string>

#include <Atlantis/DirectX12/ShaderCompiler/ShaderCompiler.h>

#include <eden/include/utility/ender_utility.h>
#include <Atlantis/DirectX12/DirectX12BaseDefine.h>
#include <eden/include/utility/StringUtility.h>
#include <Atlantis/Utility/FileUtility/FileUtility.h>
#include <Atlantis/Utility/FileUtility/FileTypeDefine.h>
#include <eden/include/system/directory/directory_manager.h>

using namespace std;

USING_ATLANTIS;

EDENS_NAMESPACE_USING;

CShaderBase::CShaderBase()
{
}

CShaderBase::~CShaderBase()
{
}

bool CShaderBase::Initialize(const FResourceInitializerBase* _Initializer)
{
	do
	{

		const FShaderBaseInitializer* initializer = PCast<const FShaderBaseInitializer*>(_Initializer);
		CHECK_RESULT_BREAK(initializer);

		m_ResourceName = initializer->Name;

		CHECK_RESULT_BREAK(CreateShader(initializer));

		return true;
	} while (0);

	Finalize();
	return false;
}

void CShaderBase::Finalize()
{
	//ReleaseD3DUniquePtr(m_ShaderBlob);
	//SafeReleaseD3DPtr(m_Impl->m_ShaderBlob);
	DeleteArray(m_ShaderData->m_ShaderCode);
	Delete(m_ShaderData);
}

PShaderCode CShaderBase::GetShaderByteCode() const
{
	//return m_ShaderBlob->GetBufferPointer();
	//if (m_ShaderData->m_ShaderBlob == nullptr)
	//{
	//	m_ShaderData->m_ShaderBlob->GetBufferPointer();
	//}
	return m_ShaderData->m_ShaderCode;
}

uint64 CShaderBase::GetBufferSize() const
{
	//return m_ShaderBlob->GetBufferSize();
	//if (m_ShaderData->m_ShaderBlob == nullptr)
	//{
	//	m_ShaderData->m_ShaderBlob->GetBufferSize();
	//}
	return m_ShaderData->m_BufferSize;
}

bool CShaderBase::CreateShader(const FShaderBaseInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	CShaderCompiler compiler = {};
	CShaderCompiler::FInitializer initializer = {};
	
	initializer.FileAndFuncHash = _Initializer->Name;
	initializer.Data = _Initializer->Data;
	initializer.Size = _Initializer->Size;
	initializer.TargetHash = _Initializer->TargetHash;

	FShaderData data = {};

	CHECK_RESULT_FALSE(compiler.ShaderCompile(&data, initializer));

	m_ShaderData = new FShaderData();
	CHECK_RESULT_FALSE(m_ShaderData);
	
	m_ShaderData->m_ShaderCode = new uint8[data.m_BufferSize];
	m_ShaderData->m_BufferSize = data.m_BufferSize;
	
	memcpy_s(
		CCast<void*>(m_ShaderData->m_ShaderCode),
		m_ShaderData->m_BufferSize,
		data.m_ShaderCode,
		data.m_BufferSize);

	return true;
}

Hash160& CShaderBase::TargetSet(FShaderBaseInitializer* _Initializer)
{
	return _Initializer->TargetHash;
}
