#pragma once


struct ID3D12Device;
struct ID3D12DescriptorHeap;
struct ID3D12Resource;

ATLANTIS_NAMESPACE_BEGIN


class CDepthStencilView
{
public:

	struct FInitializer
	{
		ID3D12Device* Device = nullptr;
		uint32 Width = 0;
		uint32 Height = 0;
		float ClearDepth = 1.0f;
	};

	bool Initialize(const FInitializer& _Initializer);

	void Finalize();

	ID3D12DescriptorHeap* GetDescriptorHeap() const { return m_MaterialDescriptorHeap; };
	ID3D12Resource* GetResource() const { return m_Resource; };


	CDepthStencilView() {};
	~CDepthStencilView() {};

private:

	bool CreateDescriptorHeap(const FInitializer& _Initializer);

	bool CreateResource(const FInitializer& _Initializer);

	bool CreateDepthStencilView(const FInitializer& _Initializer);

	ObjectPtr(ID3D12DescriptorHeap) m_MaterialDescriptorHeap = nullptr;

	ObjectPtr(ID3D12Resource) m_Resource = nullptr;

};


ATLANTIS_NAMESPACE_END