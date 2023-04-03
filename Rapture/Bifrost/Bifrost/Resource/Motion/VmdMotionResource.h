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
struct FVmdMotionPerKeyFrame;

typedef std::vector<FVmdMotionPerKeyFrame*> FVmdKeyFrameData;

class CVmdMotionResource : public EDENS_NAMESPACE::CResource
{
public:

	bool Initialize(const EDENS_NAMESPACE::FResourceInitializerBase* _Initializer) override;
	void Finalize() override;

	const FVmdKeyFrameData* GetContinuumMotionData(const Hash160& _Hash) const;
	const FVmdMotionPerKeyFrame* FindMotionDataPerFrame(const Hash160& _Hash,uint32 _KeyFrame) const;

	CVmdMotionResource();
	~CVmdMotionResource();

private:

	//typedef std::vector<FVmdMotionPerKeyFrame*> FVmdKeyFrameData;
	typedef std::map<Hash160, FVmdKeyFrameData*> FVmdMotionList;
	FVmdMotionList m_MotionList = {};

private:

	bool CreateMotionList(const FVmdMotionResourceInitializer* _Initializer);

};

BIFROST_NAMESPACE_END