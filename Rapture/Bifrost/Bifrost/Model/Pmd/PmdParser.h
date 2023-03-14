#pragma once

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
	const FPmdVertex* GetVertices() const;
	uint32 GetVertexNum() const ;
	uint32 GetVertexSize() const ;
	const uint16* GetIndicies() const;
	uint32 GetIndexNum() const;
	const FPmdMaterialData* GetMaterials() const;
	uint32 GetMaterialNum() const;
	const FPmdBoneData* GetBones() const;
	uint32 GetBoneNum() const;

	CPmdParser();
	~CPmdParser();

private:

	class Impl;
	ObjectPtr(Impl) m_Impl = nullptr;

};

BIFROST_NAMESPACE_END
//ATLANTIS_NAMESPACE_END