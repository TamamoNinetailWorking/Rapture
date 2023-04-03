#include "VmdMotionResource.h"
#include "VmdMotionResourceInitializer.h"
#include <Bifrost/Model/Vmd/VmdMotionDefine.h>
#include <Bifrost/Model/Vmd/VmdDefine.h>

#include <eden/include/utility/ender_utility.h>

#include <DirectXMath.h>

USING_BIFROST;

using namespace DirectX;

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
		if (elem.second == nullptr) { continue; }

		EDENS_NAMESPACE::Delete(elem.second);
	}

	m_MotionList.clear();
}

bool CVmdMotionResource::CreateMotionList(const FVmdMotionResourceInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(_Initializer->Motions);

	for (uint32 count = 0; count < _Initializer->MotionNum; ++count)
	{
		const FVmdMotionData& motion = _Initializer->Motions[count];

		FVmdMotionResourceData* data = new FVmdMotionResourceData();
		CHECK_RESULT_FALSE(data);

		Hash160 boneName = CHash160(motion.Name);
		
		data->FrameNo = motion.FrameNo;
		data->Quaternion = XMLoadFloat4(&(motion.Quat));

		m_MotionList[boneName] = data;
	}

	m_ResourceName = _Initializer->Name;

	return true;
}
