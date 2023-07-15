#pragma once

/*	const void* Vertices = nullptr;
uint32 VerticesSize = 0;
uint32 StrideBytes = 0;

const uint16* Indices = nullptr;
uint32 IndicesSize = 0;*/

BIFROST_NAMESPACE_BEGIN

class CPrimitivePlain
{
public:

	static const Hash160& GetName() { return NameHash; };

	static const void* GetVertices() { return Vertices; };
	static uint32 GetVerticesNum() { return VerticesNum; };
	static uint32 GetVerticesStride() { return VerticesStride; };
	
	static const uint16* GetIndices() { return Indices; };
	static uint32 GetIndicesNum() { return IndicesNum; };

private:

	static const Hash160 NameHash;

	static const void* Vertices;
	static uint32 VerticesNum;
	static uint32 VerticesStride;

	static const uint16* Indices;
	static uint32 IndicesNum;

};

BIFROST_NAMESPACE_END