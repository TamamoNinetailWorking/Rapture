#include "BasicShader.hlsli"

float4 main(VSOutput input) : SV_TARGET
{
	//return float4((float2(0,1) + input.pos.xy) * 0.5f,1,1);
	return float4(input.uv , 1.f,1.f);
}