#pragma once

#include <eden/include/Resource/ResourceInitializer.h>

ATLANTIS_NAMESPACE_BEGIN

struct FShaderBaseInitializer : public EDENS_NAMESPACE::FResourceInitializerBase
{
	// FileNameとFuncNameを結合したものを与えて、
	// セパレータで中で分離する形にしたい
	// >> そうすればHashTableに登録するものは1つになる
	Hash160 FileNameHash = {};
	Hash160 FuncNameHash = {};
protected:
	Hash160 TargetHash = {};

	friend class CShaderBase;
};

ATLANTIS_NAMESPACE_END