#pragma once

#include <memory>

struct ID3D12Device;
struct ID3D12RootSignature;

ATLANTIS_NAMESPACE_BEGIN

class CRootSignature
{
public:

	struct FInitializer
	{
		ID3D12Device* Device = nullptr;
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