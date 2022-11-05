#include "BasicShader.hlsli"

VSOutput main(
	float3 pos : POSITION,
	float2 uv : TEXCOORD
) 
{
	VSOutput output;
	output.pos = pos;
	output.svpos = float4(pos,1.f);
	output.uv = uv;
	return output;
}
