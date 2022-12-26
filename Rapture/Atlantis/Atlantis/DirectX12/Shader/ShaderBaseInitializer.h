#pragma once

#include <eden/include/Resource/ResourceInitializer.h>

ATLANTIS_NAMESPACE_BEGIN

struct FShaderBaseInitializer : public EDENS_NAMESPACE::FResourceInitializerBase
{
	// FileName��FuncName�������������̂�^���āA
	// �Z�p���[�^�Œ��ŕ�������`�ɂ�����
	// >> ���������HashTable�ɓo�^������̂�1�ɂȂ�
	Hash160 FileNameHash = {};
	Hash160 FuncNameHash = {};
protected:
	Hash160 TargetHash = {};

	friend class CShaderBase;
};

ATLANTIS_NAMESPACE_END