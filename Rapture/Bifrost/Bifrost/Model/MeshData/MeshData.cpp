#include "MeshData.h"
#include "MeshDataInitializer.h"

#include <eden/include/utility/ender_utility.h>

#include <Atlantis/DirectX12/VertexBuffer/VertexBuffer.h>
#include <Atlantis/DirectX12/IndexBuffer/IndexBuffer.h>

#include <Atlantis/DirectX12/MainDevice/MainDevice.h>

USING_BIFROST;
USING_ATLANTIS;
EDENS_NAMESPACE_USING;

bool CMeshData::Initialize(const FResourceInitializerBase* _Initializer)
{
	do
	{
		CHECK_RESULT_BREAK(CreateVertexBuffer(PCast<const FMeshResourceInitializer*>(_Initializer)));
		CHECK_RESULT_BREAK(CreateIndexBuffer(PCast<const FMeshResourceInitializer*>(_Initializer)));

		return true;
	} while (0);

	Finalize();
	return false;
}

void CMeshData::Finalize()
{
	FinalizeObject(m_IndexBuffer);
	FinalizeObject(m_VertexBuffer);
}

bool CMeshData::CreateVertexBuffer(const FMeshResourceInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(_Initializer->Device);
	CHECK_RESULT_FALSE(_Initializer->Vertices);

	m_VertexBuffer = new CVertexBuffer();
	CHECK_RESULT_FALSE(m_VertexBuffer);

	m_ResourceName = _Initializer->Name;

	CVertexBuffer::FInitializer initializer = {};
	initializer.Device = _Initializer->Device->GetDevice();
	initializer.Vertices = _Initializer->Vertices;
	initializer.StrideBytes = _Initializer->StrideBytes;
	initializer.VerticesSize = _Initializer->VerticesSize;

	return m_VertexBuffer->Initialize(initializer);
}

bool CMeshData::CreateIndexBuffer(const FMeshResourceInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	if ((_Initializer->Indices == nullptr) || (_Initializer->IndicesSize <= 0))
	{
		m_IndexBuffer = nullptr;
		return true;
	}

	CHECK_RESULT_FALSE(_Initializer->Device);

	m_IndexBuffer = new CIndexBuffer();
	CHECK_RESULT_FALSE(m_IndexBuffer);

	CIndexBuffer::FInitializer initializer = {};
	initializer.Device = _Initializer->Device->GetDevice();
	initializer.Indices = _Initializer->Indices;
	initializer.IndicesSize = _Initializer->IndicesSize;

	return m_IndexBuffer->Initialize(initializer);
}
