#include "BasicShader.hlsli"

Texture2D<float4> Texture0 : register(t0);
SamplerState Sampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float3 Color = float4(Texture0.Sample(Sampler, input.uv)).rgb;
    float Brightness = dot(Color, float3(0.2126, 0.7152, 0.0722));
	
    float4 LastColor = float4(Brightness, Brightness, Brightness, 1.0f);
    
    return LastColor;
}