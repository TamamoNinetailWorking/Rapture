#include "PmdShader.hlsli"

cbuffer Material : register(b1)
{
	float4 Diffuse;
	float4 Specular;
	float3 Ambient;
};

Texture2D<float4> Texture0 : register(t0);
Texture2D<float4> SphereMap0 : register(t1);
Texture2D<float4> SphereAddMap0 : register(t2);
Texture2D<float4> ToonMap0 : register(t3);

SamplerState Sampler : register(s0);
SamplerState ToonSampler : register(s1);

float4 main(VSOutput _Input) : SV_TARGET
{
	//float3 light = normalize(float3(1.0f,-1.0f,1.0f));
	float3 light = _Input.lightRay;
	//float3 lightColor = float3(1.0f, 1.0f, 1.0f);
	float4 lightColor = float4(_Input.lightColor,1.0f);
	float4 ambientLightColor = float4(0.5f, 0.5f, 0.5f, 1.0f);
	float brightness = dot(-light, _Input.normal);

	float3 refLight = normalize(reflect(light, _Input.normal));

	float specular = pow(saturate(dot(refLight, -_Input.eyeRay)), Specular.a);

	float2 normalUV = (_Input.vnormal.xy + float2(1.0f, -1.0f)) * float2(0.5f, -0.5f);
	//float2 sphereMapUV = _Input.vnormal.xy;

	float4 toonDif = ToonMap0.Sample(ToonSampler, float2(0, 1.0f - brightness));
	float4 diffuseColor = Diffuse * lightColor;
	diffuseColor.a = 1.0f;
	float4 specColor = float4(specular * Specular.rgb, 1.0f) * lightColor;
	//float4 bright = float4(lightColor * brightness, 1.0f);
	float4 textureColor = Texture0.Sample(Sampler, _Input.uv);
	float4 sphColor = SphereMap0.Sample(Sampler, normalUV);
	float4 spaColor = SphereAddMap0.Sample(Sampler, normalUV) * textureColor;
	float4 ambientColor = float4(Ambient, 1.0f) * textureColor * ambientLightColor;
	//ambientColor = saturate(ambientColor);
	//float4 color = bright * Diffuse * textureColor;
	float4 color = diffuseColor * textureColor;
	color = color * toonDif;
	color = color * sphColor;
	color = color + specColor;
	color = color + spaColor;
	//color = color + ambientColor;
	color = max(color,ambientColor);


	//return textureColor * diffuseColor * sphColor + specColor + spaColor;
	return color;
}