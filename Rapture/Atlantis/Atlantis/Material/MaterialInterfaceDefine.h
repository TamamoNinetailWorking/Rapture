#pragma once

ATLANTIS_NAMESPACE_BEGIN

struct FMaterialGeometryBufferBase
{
	virtual void* GetData() const { return nullptr; };
};

ATLANTIS_NAMESPACE_END