#pragma once

struct ID3D12Resource;
struct D3D12_INDEX_BUFFER_VIEW;
struct ID3D12Device;

ATLANTIS_NAMESPACE_BEGIN

class CIndexBuffer
{
public:

	struct FInitializer
	{
		ID3D12Device* Device = nullptr;
		const uint16* Indices = nullptr;
		uint32 IndicesSize = 0;
	};

	bool Initialize(const FInitializer& _Initializer);
	void Finalize();

	const D3D12_INDEX_BUFFER_VIEW* GetIndexBufferView() const { return m_View; };

	CIndexBuffer() {};
	~CIndexBuffer() {};


private:

	bool CreateIndexBuffer(const FInitializer& _Initializer);

	ObjectPtr(ID3D12Resource) m_Resource = nullptr;
	ObjectPtr(D3D12_INDEX_BUFFER_VIEW) m_View = nullptr;

};


ATLANTIS_NAMESPACE_END