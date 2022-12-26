#pragma once

EDENS_NAMESPACE_BEGIN

class CSerializer;

EDENS_NAMESPACE_END

#include <vector>
#include <Atlantis/DirectX12/DirectXPaste.h>

#include "PmdPreDefine.h"

// このクラスのPrivate部分を、Pimpl形式にしてみてもいいかも


//ATLANTIS_NAMESPACE_BEGIN
BIFROST_NAMESPACE_BEGIN

class CPmdParser
{
public:

	bool ParseData(const uint8* _Data, uint32 _Size);

	void Reset();

	// Getter
	const FPmdVertex* GetVertices() const { return m_Vertices->data(); };
	uint32 GetVertexNum() const { return SCast<uint32>(m_Vertices->size()); };
	uint32 GetVertexSize() const { return SCast<uint32>(sizeof(FPmdVertex)); };
	const uint16* GetIndicies() const { return m_Indices->data(); };
	uint32 GetIndexNum() const { return SCast<uint32>(m_Indices->size()); };
	const FPmdMaterialData* GetMaterials() const { return RCast<FPmdMaterialData*>(m_Materials->data()); };
	uint32 GetMaterialNum() const { return SCast<uint32>(m_Materials->size()); };

	CPmdParser() {};
	~CPmdParser() {};

private:

	struct FDeserializeMaterialData
	{
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
		uint16 padding = {};

		Second second = {};
	};

	union FPmdMaterialUnion
	{
		FPmdMaterialData Data = {};
		FDeserializeMaterialData desData;
	};

	typedef std::vector<FPmdVertex> FPmdVertices;
	typedef std::vector<uint16> FPmdIndicies;
	typedef std::vector<FPmdMaterialUnion> FPmdMaterials;

	bool DeserializeHeader();
	bool DeserializeVertices();
	bool DeserializerIndicies();
	bool DeserializerMaterials();

	ObjectPtr(EDENS_NAMESPACE::CSerializer) m_Serializer = nullptr;

	ObjectPtr(FPmdVertices) m_Vertices = nullptr;
	ObjectPtr(FPmdIndicies) m_Indices = nullptr;
	ObjectPtr(FPmdMaterials) m_Materials = nullptr;
	

};

BIFROST_NAMESPACE_END
//ATLANTIS_NAMESPACE_END