#include "BasicShader.hlsli"

Texture2D<float4> Texture0 : register(t0);
SamplerState Sampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	//return float4((float2(0,1) + input.pos.xy) * 0.5f,1,1);
	//return float4(input.uv , 1.f,1.f);

    float4 LastColor = float4(Texture0.Sample(Sampler, input.uv));
    //LastColor *= float4(input.uv, 1.f, 1.f);
	
	return LastColor;
	//return float4(1.0f,0.3f,0.3f,1.0f);
}