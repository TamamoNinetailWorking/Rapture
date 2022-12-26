#include "PrintUtility.h"

USING_ATLANTIS;

void PrintUtility::PrintVector(const Glue::Vector2& _vector)
{
	PRINT("x:%4.2f,y:4.2f", _vector.x, _vector.y);
}

void PrintUtility::PrintVector(const Glue::Vector3& _vector)
{
	PRINT("x:%4.2f,y:4.2f,z:4.2f", _vector.x, _vector.y,_vector.z);
}

void PrintUtility::PrintVector(const Glue::Vector4& _vector)
{
	PRINT("x:%4.2f,y:4.2f,z:4.2f,w:4.2f", _vector.x, _vector.y, _vector.z,_vector.w);
}
