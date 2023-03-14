#include "PmdParser.h"

#include <vector>

#include <eden/include/utility/ender_utility.h>

#include <Atlantis/DirectX12/DirectXPaste.h>

#include <eden/include/gadget/serializer/serializer.h>

#include <Atlantis/Utility/PrintUtility/PrintUtility.h>


EDENS_NAMESPACE_USING;
USING_ATLANTIS;
USING_BIFROST;

using namespace std;

class CPmdParser::Impl
{
public:
	struct FDeserializeMaterialData
	{
	public:
		struct First
		{
			ATLANTIS_NAMESPACE::Glue::Vector3 Diffuse = {};
			float Alpha = 0.f;
			float Specularity = 0.f;
			ATLANTIS_NAMESPACE::Glue::Vector3 SpecularColor = {};
			ATLANTIS_NAMESPACE::Glue::Vector3 AmbientColor = {};
		};

		struct Second
		{
			uint32 IndiciesNum = 0;
			char TexFilePath[TexFilePathLength] = {};
		};

		First first = {};

		uint8 ToonIndex = {};
		uint8 EdgeFlag = {};
	private:
		uint16 padding = {};
	public:
		Second second = {};
	};

	union FPmdMaterialUnion
	{
		FPmdMaterialData Data = {};
		FDeserializeMaterialData desData;
	};

	struct FDeserializeBoneData
	{
	public:
		struct First
		{
			char BoneName[BoneNameLength] = {};
			uint16 ParentNo = 0;
			uint16 NextNo = 0;

		};

		struct Second
		{
			ATLANTIS_NAMESPACE::Glue::Vector3 BasePos = {};
		};

		First first = {};
		uint8 Type = 0;

	private:
		uint8 padding1 = 0;

	public:
		uint16 IKBoneNo = 0;
		Second second = {};
	};

	union FPmdBoneUnion
	{
		FPmdBoneData Data = {};
		FDeserializeBoneData desData;
	};

	typedef std::vector<FPmdVertex> FPmdVertices;
	typedef std::vector<uint16> FPmdIndicies;
	typedef std::vector<FPmdMaterialUnion> FPmdMaterials;
	typedef std::vector<FPmdBoneUnion> FPmdBones;

	bool DeserializeHeader();
	bool DeserializeVertices();
	bool DeserializerIndicies();
	bool DeserializerMaterials();
	bool DeserializeBones();

private:

	friend class CPmdParser;

	ObjectPtr(EDENS_NAMESPACE::CSerializer) m_Serializer = nullptr;

	ObjectPtr(FPmdVertices) m_Vertices = nullptr;
	ObjectPtr(FPmdIndicies) m_Indices = nullptr;
	ObjectPtr(FPmdMaterials) m_Materials = nullptr;
	ObjectPtr(FPmdBones) m_Bones = nullptr;

};

CPmdParser::CPmdParser()
{
	m_Impl = new Impl();
}

CPmdParser::~CPmdParser()
{
	if (m_Impl)
	{
		delete m_Impl;
	}
	m_Impl = nullptr;
}

bool CPmdParser::ParseData(const uint8* _Data, uint32 _Size)
{
	if (_Data == nullptr) { return false; };
	if (_Size <= 0) { return false; };



	// Serializerを作るか
	m_Impl->m_Serializer = new CSerializer();
	if (m_Impl->m_Serializer == nullptr) { return false; };

	m_Impl->m_Serializer->Initialize(_Data, _Size);
	
	do
	{
		CHECK_RESULT_BREAK(m_Impl->DeserializeHeader());
		CHECK_RESULT_BREAK(m_Impl->DeserializeVertices());
		CHECK_RESULT_BREAK(m_Impl->DeserializerIndicies());
		CHECK_RESULT_BREAK(m_Impl->DeserializerMaterials());
		//CHECK_RESULT_BREAK(m_Impl->DeserializeBones());

		m_Impl->m_Serializer->Finalize();
		return true;
	} while (0);




	//m_Serializer->Finalize();
	Reset();
	return false;
}

void CPmdParser::Reset()
{
	EDENS_NAMESPACE::Delete(m_Impl->m_Bones);
	EDENS_NAMESPACE::Delete(m_Impl->m_Materials);

	if (m_Impl->m_Indices != nullptr)
	{
		delete m_Impl->m_Indices;
		m_Impl->m_Indices = nullptr;
	}

	if (m_Impl->m_Vertices != nullptr)
	{
		delete m_Impl->m_Vertices;
		m_Impl->m_Vertices = nullptr;
	}

	if (m_Impl->m_Serializer != nullptr)
	{
		m_Impl->m_Serializer->Finalize();
		delete m_Impl->m_Serializer;
		m_Impl->m_Serializer = nullptr;
	}
}

const FPmdVertex* CPmdParser::GetVertices() const
{
	return m_Impl->m_Vertices->data();
}

uint32 CPmdParser::GetVertexNum() const
{
	return SCast<uint32>(m_Impl->m_Vertices->size());
}

uint32 CPmdParser::GetVertexSize() const
{
	return SCast<uint32>(sizeof(FPmdVertex));
}

const uint16* CPmdParser::GetIndicies() const
{
	return m_Impl->m_Indices->data();
}

uint32 CPmdParser::GetIndexNum() const
{
	return SCast<uint32>(m_Impl->m_Indices->size());
}

const FPmdMaterialData* CPmdParser::GetMaterials() const
{
	return RCast<FPmdMaterialData*>(m_Impl->m_Materials->data());
}

uint32 CPmdParser::GetMaterialNum() const
{
	return SCast<uint32>(m_Impl->m_Materials->size());
}

const FPmdBoneData* CPmdParser::GetBones() const
{
	return RCast<FPmdBoneData*>(m_Impl->m_Bones->data());
}

uint32 CPmdParser::GetBoneNum() const
{
	return SCast<uint32>(m_Impl->m_Bones->size());
}

bool CPmdParser::Impl::DeserializeHeader()
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

bool CPmdParser::Impl::DeserializeVertices()
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

bool CPmdParser::Impl::DeserializerIndicies()
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

bool CPmdParser::Impl::DeserializerMaterials()
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

bool CPmdParser::Impl::DeserializeBones()
{
	CHECK_RESULT_FALSE(m_Serializer);

	m_Bones = new FPmdBones();
	CHECK_RESULT_FALSE(m_Bones);

	bool result = false;


	// ボーン数を取得
	uint16 boneNum = 0;
	result = m_Serializer->ReadDataBlob(&boneNum, sizeof(boneNum));
	CHECK_RESULT_FALSE(result);

	m_Bones->resize(boneNum);

	for (auto& elem : *m_Bones)
	{
		result = m_Serializer->ReadDataBlob(&elem.desData.first, sizeof(FDeserializeBoneData::First));
		CHECK_RESULT_FALSE(result);

		result = m_Serializer->ReadDataIncRef(&elem.desData.Type);
		CHECK_RESULT_FALSE(result);

		result = m_Serializer->ReadDataBlob(&elem.desData.IKBoneNo, sizeof(uint16));
		CHECK_RESULT_FALSE(result);

		result = m_Serializer->ReadDataBlob(&elem.desData.second, sizeof(FDeserializeBoneData::Second));
		CHECK_RESULT_FALSE(result);
	}

	return true;
}
