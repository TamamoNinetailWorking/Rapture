
#include <eden/include/gadget/FileLoader/FileLoader.h>
#include <Bifrost/Model/Pmd/PmdParser.h>
//#include <Atlantis/DirectX12/VertexBuffer/VertexBuffer.h>
//#include <Atlantis/DirectX12/IndexBuffer/IndexBuffer.h>
#include <Bifrost/Model/MeshData/MeshData.h>
#include <Bifrost/Model/MeshData/MeshDataInitializer.h>

#include <d3d12.h>

EDENS_NAMESPACE_USING;
USING_ATLANTIS;
USING_BIFROST;

namespace Test
{
	//CVertexBuffer* m_VertexBuffer = nullptr;
	//CIndexBuffer* m_IndexBuffer = nullptr;

	CMeshData* m_MeshData = nullptr;

	void FileLoaderTest(ID3D12Device* _Device)
	{
		//const char* fileName = "resource/mmd/UserFile/Model/初音ミク.pmd";
		//const char* fileName = "resource/mmd/UserFile/Model/初音ミクVer2.pmd";
		const char* fileName = "resource/mmd/UserFile/Model/初音ミクmetal.pmd";
		//const char* fileName = "resource/mmd/UserFile/Model/巡音ルカ.pmd";
		CFileLoader fileLoader = {};
		fileLoader.FileLoad(fileName);

		CPmdParser pmdParser = {};
		pmdParser.ParseData(fileLoader.GetData(), fileLoader.GetSize());

		fileLoader.ResetData();

#if 0
		//m_VertexBuffer = new CVertexBuffer();
		//{
		//	CVertexBuffer::FInitializer initializer = {};
		//	initializer.Device = _Device;
		//	initializer.Vertices = pmdParser.GetVertices();

		//	uint32 vertexSize = pmdParser.GetVertexSize();
		//	uint32 vertexNum = pmdParser.GetVertexNum();

		//	initializer.VerticesSize = vertexSize * vertexNum;
		//	initializer.StrideBytes = vertexSize;

		//	m_VertexBuffer->Initialize(initializer);
		//}

		//m_IndexBuffer = new CIndexBuffer();
		//{
		//	CIndexBuffer::FInitializer initializer = {};
		//	initializer.Device = _Device;
		//	initializer.Indices = pmdParser.GetIndicies();

		//	uint32 indexSize = sizeof(uint16);
		//	uint32 indexNum = pmdParser.GetIndexNum();

		//	initializer.Indices = pmdParser.GetIndicies();
		//	initializer.IndicesSize = indexSize * indexNum;
		//	
		//	m_IndexBuffer->Initialize(initializer);
		//}
#endif

		m_MeshData = new CMeshData();
		{
			//CMeshData::FInitializer initializer = {};
			FMeshResourceInitializer initializer = {};
			initializer.Device = _Device;
			initializer.Name = CHash160(fileName);
			
			initializer.Vertices = pmdParser.GetVertices();
			initializer.VerticesSize = pmdParser.GetVertexNum() * pmdParser.GetVertexSize();
			initializer.StrideBytes = pmdParser.GetVertexSize();
			
			initializer.Indices = pmdParser.GetIndicies();
			initializer.IndicesSize = SCast<uint32>(sizeof(uint16)) * pmdParser.GetIndexNum();

			m_MeshData->Initialize(&initializer);
		}

		pmdParser.Reset();
	};
};