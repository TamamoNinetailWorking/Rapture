struct VSOutput
{
	float4 svpos:SV_POSITION;
	float4 vnormal : NORMAL1;
	float3 pos:POSITION;

	float3 normal:NORMAL0;

	float3 eyeRay : VECTOR0;

	float3 lightRay : VECTOR1;

	float3 lightColor : VECTOR2;

	float2 uv : TEXCOORD0;

};