#pragma once

#include <eden/include/Resource/Resource.h>

struct ID3D12Device;

EDENS_NAMESPACE_BEGIN

struct FResourceInitializerBase;

EDENS_NAMESPACE_END

ATLANTIS_NAMESPACE_BEGIN

class CVertexBuffer;
class CIndexBuffer;

ATLANTIS_NAMESPACE_END

BIFROST_NAMESPACE_BEGIN

struct FMeshResourceInitializer;

// 頂点情報のみは許容するが、インデックス情報のみは許容しないようにしたい
// >> 頂点だけなら描画できる
class CMeshData : public EDENS_NAMESPACE::CResource
{
public:

	bool Initialize(const EDENS_NAMESPACE::FResourceInitializerBase* _Initializer) override;
	void Finalize() override;

	const ATLANTIS_NAMESPACE::CVertexBuffer* GetVertexBuffer() const { return m_VertexBuffer; };
	const ATLANTIS_NAMESPACE::CIndexBuffer* GetIndexBuffer() const { return m_IndexBuffer; };

	CMeshData() {};
	~CMeshData() {};

private:

	bool CreateVertexBuffer(const FMeshResourceInitializer* _Initializer);
	bool CreateIndexBuffer(const FMeshResourceInitializer* _Initializer);

	ObjectPtr(ATLANTIS_NAMESPACE::CVertexBuffer) m_VertexBuffer = nullptr;
	ObjectPtr(ATLANTIS_NAMESPACE::CIndexBuffer) m_IndexBuffer = nullptr;

};

BIFROST_NAMESPACE_END
//ATLANTIS_NAMESPACE_END