#pragma once

#include <Atlantis/Math/GlueMath.h>

BIFROST_NAMESPACE_BEGIN

struct FTransform
{
private:

	using Vector3 = ATLANTIS_NAMESPACE::Glue::Vector3;
	
public:

	Vector3 Position = {};
	Vector3 Rotate = {};
	Vector3 Scale = Vector3(1.0f,1.0f,1.0f);

};

BIFROST_NAMESPACE_END