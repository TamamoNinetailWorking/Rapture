#include "BasicShader.hlsli"

Texture2D<float4> Texture0 : register(t0);
Texture2D<float4> Texture1 : register(t1);
Texture2D<float4> Texture2 : register(t2);
Texture2D<float4> Texture3 : register(t3);
Texture2D<float4> Texture4 : register(t4);
SamplerState Sampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 LastColor = float4(0.f, 0.f, 0.f, 0.f);
	
    LastColor += Texture0.Sample(Sampler, input.uv);
    LastColor += Texture1.Sample(Sampler, input.uv);
    LastColor += Texture2.Sample(Sampler, input.uv);
    LastColor += Texture3.Sample(Sampler, input.uv);
    LastColor += Texture4.Sample(Sampler, input.uv);
    
    return LastColor;
}