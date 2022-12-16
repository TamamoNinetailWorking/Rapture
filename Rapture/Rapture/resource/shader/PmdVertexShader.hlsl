#include "PmdShader.hlsli"


struct VSInput
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
	min16uint2 BoneNo : BONE_NO;
	min16uint Weight : WEIGHT;
	min16uint EdgeFlag : EDGE_FLAG;
};

cbuffer ConstantBuffer0 : register(b0)
{
	matrix WMatrix;
	matrix ViewMatrix;
	matrix VPMatrix;
	matrix WVPMatrix;
	float3 EyePos;
	float padding1;
	float3 LightPos;
	float padding2;
	float3 LightColor;
	float padding3;
};

VSOutput main( VSInput _Input)
{
	VSOutput outPut;
	outPut.svpos = mul(WVPMatrix,float4(_Input.Position, 1.0f));
	outPut.pos = _Input.Position;
	//outPut.normal = _Input.Normal;
	outPut.normal = mul(WMatrix, float4(_Input.Normal, 0.0f)).xyz;
	outPut.vnormal = mul(ViewMatrix,float4(outPut.normal,0.0f));
	outPut.uv = _Input.UV;

	outPut.eyeRay = normalize(outPut.pos - EyePos);
	//outPut.eyeRay = normalize(EyePos);
	outPut.lightRay = normalize(outPut.pos - LightPos);
	//outPut.lightRay = normalize(LightPos);
	outPut.lightColor = LightColor;

	return outPut;
}