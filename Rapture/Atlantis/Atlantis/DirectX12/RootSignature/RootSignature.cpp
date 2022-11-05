#include "RootSignature.h"

#include <d3d12.h>

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>

USING_ATLANTIS;

bool CRootSignature::Initialize(const FInitializer& _Initializer)
{
	do
	{
		CHECK_RESULT_BREAK(CreateRootSignature(_Initializer));

		return true;

	} while (0);

	Finalize();
	return false;
}

void CRootSignature::Finalize()
{
	ReleaseD3DPtr(m_RootSignature);
}

CRootSignature::~CRootSignature()
{
}

bool CRootSignature::CreateRootSignature(const FInitializer& _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer.Device);

	D3D12_ROOT_SIGNATURE_DESC desc = {};
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = S_OK;

	result = D3D12SerializeRootSignature(
		&desc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&signatureBlob,
		&errorBlob);

	SafeReleaseD3DPtr(errorBlob);
	D3D_ERROR_CHECK(result);

	ID3D12RootSignature* rootSignature = nullptr;
	result = _Initializer.Device->CreateRootSignature(
		0,
		signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));

	SafeReleaseD3DPtr(signatureBlob);
	D3D_ERROR_CHECK(result);

	m_RootSignature.reset(rootSignature);

	return true;
}