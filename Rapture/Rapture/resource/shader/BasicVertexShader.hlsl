#include "BasicShader.hlsli"

cbuffer cbuff0 : register(b0)
{
	matrix wmat;
	matrix vmat;
	matrix vpmat;
	matrix wvpmat;
};

VSOutput main(
	float3 pos : POSITION,
	float2 uv : TEXCOORD
) 
{
	VSOutput output;
	output.pos = pos;
	output.svpos = mul(wvpmat,float4(pos,1.f));
	output.uv = uv;
	return output;
}
