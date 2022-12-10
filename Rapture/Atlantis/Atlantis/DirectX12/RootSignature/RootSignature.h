#pragma once

#include <memory>

#include <Atlantis/DirectX12/DirectXPaste.h>

struct ID3D12Device;
struct ID3D12RootSignature;
struct D3D12_ROOT_PARAMETER;
struct D3D12_STATIC_SAMPLER_DESC;

ATLANTIS_NAMESPACE_BEGIN

class CRootSignature
{
public:

	struct FInitializer
	{
		ID3D12Device* Device = nullptr;
		Glue::ERootSignatureFlag Flag = Glue::ERootSignatureFlag::ROOT_SIGNATURE_FLAG_NONE;
		D3D12_ROOT_PARAMETER* pParameters = nullptr;
		D3D12_STATIC_SAMPLER_DESC* pStaticSamplers = nullptr;
		uint32 NumParameters = 0;
		uint32 NumStaticSamplers = 0;
	};

	bool Initialize(const FInitializer& _Initializer);

	void Finalize();


	ID3D12RootSignature* GetRootSignature() const { return m_RootSignature.get(); };

	CRootSignature() {};
	~CRootSignature();

private:

	bool CreateRootSignature(const FInitializer& _Initializer);

	std::unique_ptr<ID3D12RootSignature> m_RootSignature = nullptr;

};


ATLANTIS_NAMESPACE_END