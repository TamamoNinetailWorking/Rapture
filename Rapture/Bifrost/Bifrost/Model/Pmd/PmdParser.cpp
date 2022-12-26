#include "PmdParser.h"

#include <vector>

#include <eden/include/gadget/serializer/serializer.h>

#include <Atlantis/Utility/PrintUtility/PrintUtility.h>


EDENS_NAMESPACE_USING;
USING_ATLANTIS;
USING_BIFROST;

using namespace std;

bool CPmdParser::ParseData(const uint8* _Data, uint32 _Size)
{
	if (_Data == nullptr) { return false; };
	if (_Size <= 0) { return false; };



	// Serializerを作るか
	m_Serializer = new CSerializer();
	if (m_Serializer == nullptr) { return false; };

	m_Serializer->Initialize(_Data, _Size);
	
	do
	{
		CHECK_RESULT_BREAK(DeserializeHeader());
		CHECK_RESULT_BREAK(DeserializeVertices());
		CHECK_RESULT_BREAK(DeserializerIndicies());
		CHECK_RESULT_BREAK(DeserializerMaterials());

		m_Serializer->Finalize();
		return true;
	} while (0);




	//m_Serializer->Finalize();
	Reset();
	return false;
}

void CPmdParser::Reset()
{
	if (m_Indices != nullptr)
	{
		delete m_Indices;
		m_Indices = nullptr;
	}

	if (m_Vertices != nullptr)
	{
		delete m_Vertices;
		m_Vertices = nullptr;
	}

	if (m_Serializer != nullptr)
	{
		m_Serializer->Finalize();
		delete m_Serializer;
		m_Serializer = nullptr;
	}
}

bool CPmdParser::DeserializeHeader()
{
	if (m_Serializer == nullptr) { return false; }

	//vector<uint8> dest = {};

	// シグネチャを取り出す
	constexpr uint8 signatureSize = 3;
#if 0
	{
		dest.resize(signatureSize);

		m_Serializer->ReadDataBlob(dest.data(), signatureSize);

		for (auto& elem : dest)
		{
			PRINT("%c", elem);
		}
		PRINT("\n");
	}
#endif

	bool result = false;
	result = m_Serializer->IncrementReadPosition(signatureSize);
	CHECK_RESULT_FALSE(result);

	// ヘッダーを取り出す
	{
		constexpr uint8 ModelNameSize = 20;
		constexpr uint16 CommentSize = 256;

		struct PmdHeader
		{
			float Version = 0.f;
			char ModelName[ModelNameSize] = {};
			char Comment[CommentSize] = {};
		};
		constexpr uint32 pmdHeaderSize = sizeof(PmdHeader);
#if 0
		PmdHeader header = {};

		m_Serializer->ReadDataBlob(&header, sizeof(PmdHeader));

		PRINT("Version::%4.2f\n", header.Version);
		PRINT("ModelName::%s\n", header.ModelName);
		PRINT("Comment::%s\n", header.Comment);
#endif

		result = m_Serializer->IncrementReadPosition(pmdHeaderSize);
		CHECK_RESULT_FALSE(result);
	}

	

	return true;
}

bool CPmdParser::DeserializeVertices()
{
	if (m_Serializer == nullptr) { return false; }

	m_Vertices = new FPmdVertices();
	if (m_Vertices == nullptr) { return false; }

	bool result = false;

	// 頂点数を取得
	uint32 vertexNum = 0;
	result = m_Serializer->ReadDataBlob(&vertexNum, sizeof(vertexNum));
	CHECK_RESULT_FALSE(result);

	//PRINT("VertexNum:%u \n", vertexNum);

	m_Vertices->resize(vertexNum);

	// 頂点サイズの定義
	constexpr uint32 realSize = 38;
	constexpr uint32 structSize = sizeof(FPmdVertex);

	//uint32 index = 0;
	// 頂点情報を取得
	for (auto& vertex : *m_Vertices)
	{
		result = m_Serializer->ReadDataBlob(&vertex, structSize, realSize);
		CHECK_RESULT_FALSE(result);

		//{
		//	PRINT("Vertex[%u]\n", index);

		//	PrintUtility::PrintVector(vertex.Pos);
		//	PrintUtility::PrintVector(vertex.Normal);
		//	PrintUtility::PrintVector(vertex.UV);

		//	++index;
		//}
	}

	return true;
}

bool CPmdParser::DeserializerIndicies()
{
	CHECK_RESULT_FALSE(m_Serializer);

	m_Indices = new FPmdIndicies();
	CHECK_RESULT_FALSE(m_Indices);

	bool result = false;

	// インデックスの数を取得
	uint32 indexNum = 0;
	result = m_Serializer->ReadDataBlob(&indexNum, sizeof(indexNum));
	m_Indices->resize(indexNum);
	CHECK_RESULT_FALSE(result);

	//PRINT("IndexNum: %u\n", indexNum);

	uint32 indiciesNum = indexNum * sizeof(uint16);

	// インデックス情報を一括で取得
	result = m_Serializer->ReadDataBlob(m_Indices->data(), indiciesNum);
	CHECK_RESULT_FALSE(result);

	return true;
}

bool CPmdParser::DeserializerMaterials()
{
	CHECK_RESULT_FALSE(m_Serializer);

	m_Materials = new FPmdMaterials();
	CHECK_RESULT_FALSE(m_Materials);

	bool result = false;


	// マテリアル数を取得
	uint32 materialNum = 0;
	result = m_Serializer->ReadDataBlob(&materialNum, sizeof(materialNum));
	CHECK_RESULT_FALSE(result);

	m_Materials->resize(materialNum);

	// マテリアルデータを取得
	for (auto& elem : *m_Materials)
	{
		result = m_Serializer->ReadDataBlob(&elem.desData.first, sizeof(FDeserializeMaterialData::First));
		CHECK_RESULT_FALSE(result);

		result = m_Serializer->ReadDataIncRef(&elem.desData.ToonIndex);
		CHECK_RESULT_FALSE(result);
		result = m_Serializer->ReadDataIncRef(&elem.desData.EdgeFlag);
		CHECK_RESULT_FALSE(result);

		result = m_Serializer->ReadDataBlob(&elem.desData.second, sizeof(FDeserializeMaterialData::Second));
		CHECK_RESULT_FALSE(result);
	}


	return true;
}
