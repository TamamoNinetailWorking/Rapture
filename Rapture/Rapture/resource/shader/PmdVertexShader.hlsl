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
	// SceneData
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

	// BoneMatrix
	matrix Bones[256];
};

VSOutput main( VSInput _Input)
{
	VSOutput outPut;

	float4 position = float4(_Input.Position, 1.0f);

	float weight = _Input.Weight / 100.0f;
	matrix boneMat = Bones[_Input.BoneNo[0]] * weight + Bones[_Input.BoneNo[1]] * (1 - weight);


	position = mul(boneMat, position);

	outPut.svpos = mul(WVPMatrix,position);
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

	// BoneNo[0]が取れているか確認 >> 白飛びすることなく色が乗算されていそうなので、大丈夫だろう
	//float l2 = _Input.BoneNo[0] / 256;
	//outPut.lightColor = float3(l2, l2, l2);

	return outPut;
}