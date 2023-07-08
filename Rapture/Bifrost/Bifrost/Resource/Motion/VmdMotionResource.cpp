#include "VmdMotionResource.h"
#include "VmdMotionResourceInitializer.h"
#include <Bifrost/Model/Vmd/VmdMotionDefine.h>
#include <Bifrost/Model/Vmd/VmdDefine.h>

#include <eden/include/utility/ender_utility.h>

#include <DirectXMath.h>

USING_BIFROST;

using namespace DirectX;

constexpr uint32 KeyFrameDataInitSize = 32;

CVmdMotionResource::CVmdMotionResource()
{
}

CVmdMotionResource::~CVmdMotionResource()
{
}

bool CVmdMotionResource::Initialize(const EDENS_NAMESPACE::FResourceInitializerBase* _Initializer)
{
	do
	{
		const FVmdMotionResourceInitializer* initializer = PCast<const FVmdMotionResourceInitializer*>(_Initializer);
		CHECK_RESULT_BREAK(_Initializer);

		CHECK_RESULT_BREAK(CreateMotionList(initializer));

		return true;
	} while (0);

	Finalize();
	return false;
}

void CVmdMotionResource::Finalize()
{
	for (auto& elem : m_MotionList)
	{
		for (auto& data : *(elem.second))
		{
			EDENS_NAMESPACE::Delete(data);
		}

		elem.second->clear();
		EDENS_NAMESPACE::Delete(elem.second);
	}

	m_MotionList.clear();
}

const FVmdKeyFrameData* CVmdMotionResource::GetContinuumMotionData(const Hash160& _Hash) const
{
	auto itr = m_MotionList.find(_Hash);
	if (itr == m_MotionList.end()) { return nullptr; }
	return itr->second;
}

const FVmdMotionPerKeyFrame* CVmdMotionResource::FindCurrentMotionDataPerFrame(const Hash160& _Hash, uint32 _KeyFrame) const
{
	//const FVmdKeyFrameData* motionData = GetContinuumMotionData(_Hash);
	//auto itr = m_MotionList.find(_Hash);
	//if (itr == m_MotionList.end()) { return nullptr; }
	//const FVmdKeyFrameData* motionData =  itr->second;
	//if (motionData == nullptr) { return nullptr; }
	//
	//auto findItr = std::find_if(
	//	motionData->rbegin(),
	//	motionData->rend(),
	//	[_KeyFrame](const FVmdMotionPerKeyFrame* _Motion)
	//	{
	//		if (_Motion == nullptr) { return false; }
	//		return _Motion->FrameNo <= _KeyFrame;
	//	});
	//if (findItr == motionData->rend()) { return nullptr; }

	//return (*findItr);

	FindCondition findCondition = [_KeyFrame](const FVmdMotionPerKeyFrame* _Motion)
	{
		if (_Motion == nullptr) { return false; }
		return _Motion->FrameNo <= _KeyFrame;
	};

	return FindMotionData(_Hash, _KeyFrame, findCondition);
}

const FVmdMotionPerKeyFrame* CVmdMotionResource::FindNextMotionDataPerFrame(const Hash160& _Hash, uint32 _KeyFrame) const
{
	FindCondition findCondition = [_KeyFrame](const FVmdMotionPerKeyFrame* _Motion)
	{
		if (_Motion == nullptr) { return false; }
		return _Motion->FrameNo < _KeyFrame;
	};

	//return FindMotionData(_Hash,_KeyFrame,findCondition);

	auto itr = m_MotionList.find(_Hash);
	if (itr == m_MotionList.end()) { return nullptr; }
	const FVmdKeyFrameData* motionData = itr->second;
	if (motionData == nullptr) { return nullptr; }

	auto findItr = std::find_if(
		motionData->rbegin(),
		motionData->rend(),
		findCondition);
	auto revItrBase = findItr.base();
	if (revItrBase == motionData->end()) { return nullptr; }

	return (*revItrBase);
}

bool CVmdMotionResource::CreateMotionList(const FVmdMotionResourceInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(_Initializer->Motions);

	for (uint32 count = 0; count < _Initializer->MotionNum; ++count)
	{
		const FVmdMotionData& motion = _Initializer->Motions[count];

		FVmdMotionPerKeyFrame* data = new FVmdMotionPerKeyFrame();
		CHECK_RESULT_FALSE(data);

		Hash160 boneName = CHash160(motion.Name);

		if (m_MotionList.find(boneName) == m_MotionList.end())
		{
			FVmdKeyFrameData* keyFrameData = new FVmdKeyFrameData();
			keyFrameData->reserve(KeyFrameDataInitSize);
			m_MotionList[boneName] = keyFrameData;
		}
		FVmdKeyFrameData* keyFrameData = m_MotionList[boneName];

		if (motion.FrameNo >= keyFrameData->capacity())
		{
			keyFrameData->reserve(motion.FrameNo);
		}

		data->FrameNo = motion.FrameNo;
		data->Quaternion = XMLoadFloat4(&(motion.Quat));

		keyFrameData->push_back(data);
		//(*keyFrameData)[motion.FrameNo] = data;
	}

	m_ResourceName = _Initializer->Name;

	return true;
}

const FVmdMotionPerKeyFrame* CVmdMotionResource::FindMotionData(const Hash160& _Hash, uint32 _KeyFrame, const FindCondition& _Finder) const
{
	auto itr = m_MotionList.find(_Hash);
	if (itr == m_MotionList.end()) { return nullptr; }
	const FVmdKeyFrameData* motionData = itr->second;
	if (motionData == nullptr) { return nullptr; }

	auto findItr = std::find_if(
		motionData->rbegin(),
		motionData->rend(),
		_Finder);
	if (findItr == motionData->rend()) { return nullptr; }

	return (*findItr);
}
