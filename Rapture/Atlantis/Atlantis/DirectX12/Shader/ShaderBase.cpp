#include "ShaderBase.h"

//#include <locale.h>
#include <d3dcompiler.h>
#include <string>

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>
#include <Atlantis/DirectX12/Utility/DirectXUtility.h>

using namespace std;

USING_ATLANTIS;

bool CShaderBase::Initialize(const FInitializerBase* _Initializer)
{
	do
	{
		CHECK_RESULT_BREAK(_Initializer);
		CHECK_RESULT_BREAK(CompileShaderFromFile(PCast<const FInitializerMiddle*>(_Initializer)));
		
		return true;
	} while (0);

	Finalize();
	return false;
}

void CShaderBase::Finalize()
{
	ReleaseD3DPtr(m_ShaderBlob);
}

CShaderBase::~CShaderBase()
{
	CResource::~CResource();
}

bool CShaderBase::CompileShaderFromFile(const FInitializerMiddle* _Initializer)
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

	error = Utility::ToWString(fileName, fileLength, RHash160(_Initializer->FileNameHash), length);

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

	m_ShaderBlob.reset(shader);

	return true;
}

Hash160& CShaderBase::TargetSet(FInitializerMiddle* _Initializer)
{
	return _Initializer->TargetHash;
}
