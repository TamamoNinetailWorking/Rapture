#include "MathLibrary.h"

#include <DirectXMath.h>



USING_ATLANTIS;

using namespace DirectX;

Glue::FVector ATLANTIS_NAMESPACE::FMathLibrary::VectorSet(float x, float y, float z, float w)
{
	return XMVectorSet(x,y,z,w);
}

Glue::FMatrix ATLANTIS_NAMESPACE::FMathLibrary::MatrixTranslation(float x, float y, float z)
{
	return XMMatrixTranslation(x,y,z);
}

Glue::FMatrix ATLANTIS_NAMESPACE::FMathLibrary::MatrixTranslation(const FVector3& Translation)
{
	return MatrixTranslation(Translation.x,Translation.y,Translation.z);
}

Glue::FMatrix ATLANTIS_NAMESPACE::FMathLibrary::MatrixRotationRollPitchYaw (float x, float y, float z)
{
	return XMMatrixRotationRollPitchYaw(x, y, z);
}

Glue::FMatrix ATLANTIS_NAMESPACE::FMathLibrary::MatrixRotationRollPitchYaw(const FVector3& Rotator)
{
	return MatrixRotationRollPitchYaw(Rotator.x,Rotator.y,Rotator.z);
}

Glue::FMatrix ATLANTIS_NAMESPACE::FMathLibrary::MatrixScaling(float x, float y, float z)
{
	return XMMatrixScaling(x,y,z);
}

Glue:: FMatrix ATLANTIS_NAMESPACE::FMathLibrary::MatrixScaling(const FVector3& Scaler)
{
	return MatrixScaling(Scaler.x,Scaler.y,Scaler.z);
}

Glue::FMatrix ATLANTIS_NAMESPACE::FMathLibrary::MatrixTranspose(const FMatrix& InMatrix)
{
	return XMMatrixTranspose(InMatrix);
}

Glue::FMatrix ATLANTIS_NAMESPACE::FMathLibrary::MatrixPerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)
{
	return XMMatrixPerspectiveFovLH(FovAngleY,AspectRatio,NearZ,FarZ);
}
