#include "BasicShader.hlsli"

cbuffer BlurParameter : register(b0)
{
    float BrightThreshold;
    int SampleCount;
    float4 Offset[16];
};

Texture2D<float4> Texture0 : register(t0);
Texture2D<float4> Texture1 : register(t1);
SamplerState Sampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 LastColor = float4(0.f, 0.f, 0.f, 0.f);
    
    float BrightColor = Texture1.Sample(Sampler, input.uv).r;
        
    if(BrightColor < BrightThreshold)
    {
        return LastColor;
    }
    
    // 一旦unrollしておくが、別のサンプリング処理も検討したい
    [unroll]
    for (int i = 0; i < SampleCount; ++i)
    {
        LastColor += Offset[i].z * Texture0.Sample(Sampler, input.uv + Offset[i].xy);
    }
    
    LastColor.a = 1.0f;
	
    return LastColor;
}