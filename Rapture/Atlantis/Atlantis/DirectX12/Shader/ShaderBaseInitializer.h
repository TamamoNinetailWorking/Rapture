#pragma once

#include <eden/include/Resource/ResourceInitializer.h>

ATLANTIS_NAMESPACE_BEGIN

struct FShaderBaseInitializer : public EDENS_NAMESPACE::FResourceInitializerBase
{
	// FileName‚ÆFuncName‚ðŒ‹‡‚µ‚½‚à‚Ì‚ð—^‚¦‚ÄA
	// ƒZƒpƒŒ[ƒ^‚Å’†‚Å•ª—£‚·‚éŒ`‚É‚µ‚½‚¢
	// >> ‚»‚¤‚·‚ê‚ÎHashTable‚É“o˜^‚·‚é‚à‚Ì‚Í1‚Â‚É‚È‚é
	Hash160 FileNameHash = {};
	Hash160 FuncNameHash = {};
protected:
	Hash160 TargetHash = {};

	friend class CShaderBase;
};

ATLANTIS_NAMESPACE_END