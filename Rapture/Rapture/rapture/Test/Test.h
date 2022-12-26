#pragma once

ATLANTIS_NAMESPACE_BEGIN

//class CVertexBuffer;
//class CIndexBuffer;
//class CMeshData;

ATLANTIS_NAMESPACE_END

BIFROST_NAMESPACE_BEGIN

class CMeshData;

BIFROST_NAMESPACE_END

namespace Test 
{
	void SHA1TestMain();

	void MatrixVectorTestMain();

	void FileLoaderTest(struct ID3D12Device* _Device);

	//extern ATLANTIS_NAMESPACE::CVertexBuffer* m_VertexBuffer;
	//extern ATLANTIS_NAMESPACE::CIndexBuffer* m_IndexBuffer;
	extern BIFROST_NAMESPACE::CMeshData* m_MeshData;
};