#include "ShaderBase.h"
#include "ShaderBaseInitializer.h"

//#include <locale.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <string>

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>
#include <eden/include/utility/StringUtility.h>

using namespace std;

USING_ATLANTIS;

EDENS_NAMESPACE_USING;

bool CShaderBase::Initialize(const FResourceInitializerBase* _Initializer)
{
	do
	{
		CHECK_RESULT_BREAK(_Initializer);
		CHECK_RESULT_BREAK(CompileShaderFromFile(PCast<const FShaderBaseInitializer*>(_Initializer)));

		return true;
	} while (0);

	Finalize();
	return false;
}

void CShaderBase::Finalize()
{
	//ReleaseD3DUniquePtr(m_ShaderBlob);
	SafeReleaseD3DPtr(m_ShaderBlob);
}

CShaderBase::ShaderCode CShaderBase::GetShaderBytecod() const
{
	return m_ShaderBlob->GetBufferPointer();
}

uint64 CShaderBase::GetBufferSize() const
{
	return m_ShaderBlob->GetBufferSize();
}

CShaderBase::~CShaderBase()
{
	CResource::~CResource();
}

bool CShaderBase::CompileShaderFromFile(const FShaderBaseInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	const uint32 fileLength = 512;
	WCHAR fileName[fileLength] = {};
	size_t length = 0;
	int32 error = 0;

	ID3DBlob* shader = nullptr;
	ID3DBlob* errorBlob = nullptr;

	//setlocale(LC_ALL, "japanese");
	//error = mbstowcs_s(&length, fileName, RHash160(_Initializer->FileNameHash), _TRUNCATE);

	error = StringUtility::ToWString(fileName, fileLength, RHash160(_Initializer->FileNameHash), length);

	if (error == EINVAL) { return false; }

	HRESULT result = D3DCompileFromFile(
		fileName,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		RHash160(_Initializer->FuncNameHash),
		RHash160(_Initializer->TargetHash),
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

	//m_ShaderBlob.reset(shader);
	m_ShaderBlob = shader;

	// FileNameとFuncNameを結合したものを与えて、
	// セパレータで中で分離する形にしたい
	// >> そうすればHashTableに登録するものは1つになる
	string fn = RHash160(_Initializer->FileNameHash);
	string fcn = RHash160(_Initializer->FuncNameHash);
	m_ResourceName = CHash160(fn + fcn);

	return true;
}

Hash160& CShaderBase::TargetSet(FShaderBaseInitializer* _Initializer)
{
	return _Initializer->TargetHash;
}
