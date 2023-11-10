#pragma once

#include "GlueMath.h"

ATLANTIS_NAMESPACE_BEGIN


class FMathLibrary
{
public:

	using FMatrix = Glue::FMatrix;
	using FVector = Glue::FVector;
	using FMatrix44 = Glue::Matrix44;
	using FMatrix43 = Glue::Matrix43;
	using FMatrix34 = Glue::Matrix34;
	using FVector2 = Glue::Vector2;
	using FVector3 = Glue::Vector3;
	using FVector4 = Glue::Vector4;

public:

	/**
	 * Vector
	 */

	static FVector VectorSet(float x, float y, float z, float w);

public:

	/**
	 * Matrix
	 */

	static FMatrix MatrixTranslation(float x, float y, float z);
	static FMatrix MatrixTranslation(const FVector3& Translation);

	static FMatrix MatrixRotationRollPitchYaw(float x, float y, float z);
	static FMatrix MatrixRotationRollPitchYaw(const FVector3& Rotator);

	static FMatrix MatrixScaling(float x, float y, float z);
	static FMatrix MatrixScaling(const FVector3& Scaler);

	static FMatrix MatrixTranspose(const FMatrix& InMatrix);

	static FMatrix MatrixPerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ);
	

};


ATLANTIS_NAMESPACE_END