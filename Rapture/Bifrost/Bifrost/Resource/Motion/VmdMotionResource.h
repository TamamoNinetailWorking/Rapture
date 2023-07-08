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
	const FVmdMotionPerKeyFrame* FindCurrentMotionDataPerFrame(const Hash160& _Hash,uint32 _KeyFrame) const;
	const FVmdMotionPerKeyFrame* FindNextMotionDataPerFrame(const Hash160& _Hash, uint32 _KeyFrame) const;

	uint32 GetEndFrameNo() const { return m_EndFrameNo; }

	CVmdMotionResource();
	~CVmdMotionResource();

private:

	//typedef std::vector<FVmdMotionPerKeyFrame*> FVmdKeyFrameData;
	typedef std::map<Hash160, FVmdKeyFrameData*> FVmdMotionList;
	
	FVmdMotionList m_MotionList = {};

	typedef std::function<bool(const FVmdMotionPerKeyFrame*)> FindCondition;

private:

	bool CreateMotionList(const FVmdMotionResourceInitializer* _Initializer);

	const FVmdMotionPerKeyFrame* FindMotionData(const Hash160& _Hash, uint32 _KeyFrame, const FindCondition& _Finder) const;

private:

	uint32 m_EndFrameNo = 0;

};

BIFROST_NAMESPACE_END