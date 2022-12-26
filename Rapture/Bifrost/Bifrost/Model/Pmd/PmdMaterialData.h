#pragma once

#include <eden/include/Resource/Resource.h>

struct ID3D12DescriptorHeap;


EDENS_NAMESPACE_BEGIN

struct FResourceInitializerBase;

EDENS_NAMESPACE_END

//#include "PmdPreDefine.h"

BIFROST_NAMESPACE_BEGIN

class CPmdMaterialData : public EDENS_NAMESPACE::CResource
{
public:

	bool Initialize(const  EDENS_NAMESPACE::FResourceInitializerBase* _Initializer) override;
	void Finalize() override;



	CPmdMaterialData() {};
	~CPmdMaterialData() {};


private:

	ObjectPtr(ID3D12DescriptorHeap) m_DescriptorHeap = nullptr;

};

BIFROST_NAMESPACE_END