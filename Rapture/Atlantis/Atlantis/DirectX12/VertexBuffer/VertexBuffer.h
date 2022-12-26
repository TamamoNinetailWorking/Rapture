#pragma once

struct ID3D12Resource;
struct D3D12_VERTEX_BUFFER_VIEW;
struct ID3D12Device;

ATLANTIS_NAMESPACE_BEGIN

class CVertexBuffer
{
public:

	struct FInitializer
	{		
		ID3D12Device* Device = nullptr;
		const void* Vertices = nullptr;
		uint32 VerticesSize = 0;
		uint32 StrideBytes = 0;
	};

	bool Initialize(const FInitializer& _Initializer);
	void Finalize();

	const D3D12_VERTEX_BUFFER_VIEW* GetVertexBufferView() const { return m_View; };

	CVertexBuffer() {};
	~CVertexBuffer() {};


private:

	bool CreateVertexBuffer(const FInitializer& _Initializer);

	ObjectPtr(ID3D12Resource) m_Resource = nullptr;
	ObjectPtr(D3D12_VERTEX_BUFFER_VIEW) m_View = nullptr;

};

ATLANTIS_NAMESPACE_END