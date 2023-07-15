#include "ShaderCompiler.h"
#include <Atlantis/DirectX12/Shader/ShaderData.h>

#include <Atlantis/Utility/FileUtility/FileTypeDefine.h>
#include <Atlantis/Utility/FileUtility/FileUtility.h>
#include <eden/include/utility/StringUtility.h>

#include <d3d12.h>
#include <d3dcompiler.h>

USING_ATLANTIS;
EDENS_NAMESPACE_USING;
using namespace std;

using String = std::string;

class CShaderCompiler::Impl 
{
public:

	Impl() {};
	~Impl() {};

	bool CompileShaderFromCso(FShaderData* _DestData, const CShaderCompiler::FInitializer& _Initializer) const;
	bool CompileShaderFromMemory(FShaderData* _DestData, const CShaderCompiler::FInitializer& _Initializer) const;
	bool CompileShaderFromFile(FShaderData* _DestData, const CShaderCompiler::FInitializer& _Initializer) const;

	String m_FileName = {};
	String m_FuncName = {};

};

bool CShaderCompiler::ShaderCompile(FShaderData* _DestData, const CShaderCompiler::FInitializer& _Initializer)
{
	bool check = _Initializer.FileAndFuncHash != INVALID_HASH;
	CHECK_RESULT_FALSE(check);

	const string& name = RHash160(_Initializer.FileAndFuncHash);
	pair<string, string> ret = StringUtility::SeparateText(name, '*');
	m_Impl->m_FileName = ret.first;
	m_Impl->m_FuncName = ret.second;

	// cso
	if (m_Impl->CompileShaderFromCso(_DestData, _Initializer))
	{
		return true;
	}

	// memory
	if (m_Impl->CompileShaderFromMemory(_DestData, _Initializer))
	{
		return true;
	}

	return m_Impl->CompileShaderFromFile(_DestData,_Initializer);
}

CShaderCompiler::CShaderCompiler()
{
	m_Impl = new Impl();
}

CShaderCompiler::~CShaderCompiler()
{
	if (m_Impl)
	{
		delete m_Impl;
	}
	m_Impl = nullptr;
}

bool CShaderCompiler::Impl::CompileShaderFromCso(FShaderData* _DestData, const CShaderCompiler::FInitializer& _Initializer) const
{
	if (!FileUtility::CompareExtensionFromPath(m_FileName, FileUtility::EFileExtensionType::FILE_TYPE_CSO))
	{
		return false;
	}

	_DestData->m_ShaderCode = _Initializer.Data;
	_DestData->m_BufferSize = _Initializer.Size;
	//_DestData->m_ShaderBlob = nullptr;
	return true;
}

bool CShaderCompiler::Impl::CompileShaderFromMemory(FShaderData* _DestData, const CShaderCompiler::FInitializer& _Initializer) const
{
	if (_Initializer.Data == nullptr)
	{
		return false;
	}

	HRESULT result = {};

	ID3DBlob* shader = nullptr;
	ID3DBlob* errorBlob = nullptr;

	string tagetHash = RHash160(_Initializer.TargetHash);

	result = D3DCompile(
		_Initializer.Data,
		_Initializer.Size,
		m_FileName.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		m_FuncName.c_str(),
		RHash160(_Initializer.TargetHash),
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&shader,
		&errorBlob);

	if (FAILED(result))
	{
		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
		{
			PRINT("ファイルが存在しません。");
		}
		else
		{
			string errorString;
			errorString.resize(errorBlob->GetBufferSize());

			copy_n(PCast<char*>(errorBlob->GetBufferPointer()), errorBlob->GetBufferSize(), errorString.begin());
			errorString += "\n";

			PRINT(errorString.c_str());

			errorBlob->Release();
		}

		return false;
	}

	//_DestData->m_ShaderBlob = shader;
	_DestData->m_ShaderCode = shader->GetBufferPointer();
	_DestData->m_BufferSize = shader->GetBufferSize();

	return true;
}

bool CShaderCompiler::Impl::CompileShaderFromFile(FShaderData* _DestData, const CShaderCompiler::FInitializer& _Initializer) const
{

	HRESULT result = {};

	const uint32 fileLength = 512;
	WCHAR fileName[fileLength] = {};
	size_t length = 0;
	int32 error = 0;

	ID3DBlob* shader = nullptr;
	ID3DBlob* errorBlob = nullptr;

	error = StringUtility::ToWString(fileName, fileLength, m_FileName.c_str(), length);

	if (error == EINVAL) { return false; }

	result = D3DCompileFromFile(
		fileName,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		m_FuncName.c_str(),
		RHash160(_Initializer.TargetHash),
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&shader,
		&errorBlob);

	if (FAILED(result))
	{
		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
		{
			PRINT("ファイルが存在しません。\n");
		}
		else
		{
			if (errorBlob == nullptr)
			{
				PRINT("ファイル形式で弾かれている？ %s\n",m_FileName.c_str());
				return false;
			}
			string errorString;
			errorString.resize(errorBlob->GetBufferSize());

			copy_n(PCast<char*>(errorBlob->GetBufferPointer()), errorBlob->GetBufferSize(), errorString.begin());
			errorString += "\n";

			PRINT(errorString.c_str());

			errorBlob->Release();
		}

		return false;
	}

	//_DestData->m_ShaderBlob = shader;
	_DestData->m_ShaderCode = shader->GetBufferPointer();
	_DestData->m_BufferSize = shader->GetBufferSize();

	return true;
}
