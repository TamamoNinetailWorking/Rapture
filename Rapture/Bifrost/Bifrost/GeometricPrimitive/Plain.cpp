#include "Plain.h"

#include <Atlantis/DirectX12/GlueMath.h>

USING_BIFROST;
USING_ATLANTIS;

struct PlainVertex
{
	Glue::Vector3 Pos = {};
	Glue::Vector2 UV = {};
};

constexpr uint32 Stride = sizeof(PlainVertex);

static constexpr PlainVertex PlainData[] =
{
	//{{ -1.f,-1.f,0.1f },{0.f,1.f}},	//左下
	//{{ -1.f, 1.f,0.1f },{0.f,0.f}},	//左上
	//{{  1.f,-1.f,0.1f },{1.f,1.f}},	//右下
	//{{  1.f, 1.f,0.1f },{1.f,0.f}},	//右上
	{{ -0.5f,-0.5f,0.1f },{0.f,1.f}},	//左下
	{{ -0.5f, 0.5f,0.1f },{0.f,0.f}},	//左上
	{{  0.5f,-0.5f,0.1f },{1.f,1.f}},	//右下
	{{  0.5f, 0.5f,0.1f },{1.f,0.f}},	//右上
};

constexpr uint32 Size = sizeof(PlainData);

static constexpr uint16 PlainIndices[] =
{
	0,
	1,
	2,
	1,
	3,
	2,
};

const Hash160 CPrimitivePlain::NameHash = CHash160("PrimitivePlain");

const void* CPrimitivePlain::Vertices = PlainData;
uint32 CPrimitivePlain::VerticesNum = sizeof(PlainData) / Stride;
uint32 CPrimitivePlain::VerticesStride = Stride;

const uint16* CPrimitivePlain::Indices = PlainIndices;
uint32 CPrimitivePlain::IndicesNum = sizeof(PlainIndices) / sizeof(uint16);