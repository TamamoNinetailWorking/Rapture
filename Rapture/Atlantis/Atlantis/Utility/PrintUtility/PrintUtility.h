#pragma once

//#include <Atlantis/DirectX12/DirectXPaste.h>
#include <Atlantis/DirectX12/GlueMath.h>

ATLANTIS_NAMESPACE_BEGIN

namespace PrintUtility
{
	void PrintVector(const Glue::Vector2& _vector);
	void PrintVector(const Glue::Vector3& _vector);
	void PrintVector(const Glue::Vector4& _vector);
};

ATLANTIS_NAMESPACE_END