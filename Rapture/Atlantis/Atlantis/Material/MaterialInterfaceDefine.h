#pragma once

ATLANTIS_NAMESPACE_BEGIN

struct FMaterialGeometryBufferBase
{
	virtual FMaterialGeometryBufferBase* GetData() { return this; };
};

ATLANTIS_NAMESPACE_END