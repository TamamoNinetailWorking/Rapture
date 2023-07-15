#pragma once

#include <DirectXmath.h>

ATLANTIS_NAMESPACE_BEGIN

// 算術演算系
namespace Glue
{

	typedef DirectX::XMFLOAT2 Vector2;
	typedef DirectX::XMFLOAT3 Vector3;
	typedef DirectX::XMFLOAT4 Vector4;


	typedef DirectX::XMFLOAT3X4 Matrix34;
	typedef DirectX::XMFLOAT4X3 Matrix43;
	typedef DirectX::XMFLOAT4X4 Matrix44;

	typedef DirectX::XMVECTOR FVector;
	typedef DirectX::XMMATRIX FMatrix;

};

ATLANTIS_NAMESPACE_END