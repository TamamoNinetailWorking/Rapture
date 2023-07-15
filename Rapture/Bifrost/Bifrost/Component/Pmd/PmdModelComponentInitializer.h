#pragma once

#include <Bifrost/Component/ComponentInitializer.h>

BIFROST_NAMESPACE_BEGIN

struct FPmdModelComponentInitializer : public FComponentInitializerBase
{
	Hash160 FileName = {};
	Hash160 ToonMapDirectory = {};
	Hash160 PsoName = {};
};

BIFROST_NAMESPACE_END