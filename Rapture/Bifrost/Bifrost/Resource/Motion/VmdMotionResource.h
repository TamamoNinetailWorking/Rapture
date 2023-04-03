#pragma once

#include <map>

#include <eden/include/Resource/Resource.h>
#include <eden/include/Resource/ResourceManagerPreDefine.h>


struct ID3D12Resource;

EDENS_NAMESPACE_BEGIN

struct FResourceInitializerBase;

EDENS_NAMESPACE_END


BIFROST_NAMESPACE_BEGIN

struct FVmdMotionResourceInitializer;
struct FVmdMotionResourceData;

class CVmdMotionResource : public EDENS_NAMESPACE::CResource
{
public:

	bool Initialize(const EDENS_NAMESPACE::FResourceInitializerBase* _Initializer) override;
	void Finalize() override;

	CVmdMotionResource();
	~CVmdMotionResource();

private:

	typedef std::map<Hash160, FVmdMotionResourceData*> FVmdMotionList;
	FVmdMotionList m_MotionList = {};

private:

	bool CreateMotionList(const FVmdMotionResourceInitializer* _Initializer);

};

BIFROST_NAMESPACE_END