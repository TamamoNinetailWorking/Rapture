#include "PmdSkeleton.h"
#include "PmdSkeletonInitializer.h"
#include "PmdPreDefine.h"
#include "PmdSkeletonDefine.h"

#include <eden/include/utility/ender_utility.h>

#include <DirectXMath.h>



USING_BIFROST;

USING_ATLANTIS;
using namespace Glue;
using namespace DirectX;

CPmdSkeleton::CPmdSkeleton()
{
}

CPmdSkeleton::~CPmdSkeleton()
{
}

bool CPmdSkeleton::Initialize(const FPmdSkeletonInitializer* _Initializer)
{
	do
	{
		CHECK_RESULT_FALSE(_Initializer);

		m_Matrices = new FBoneMatrices();
		CHECK_RESULT_BREAK(m_Matrices);

		m_Table = new FBoneNodeTable();
		CHECK_RESULT_BREAK(m_Table);

		CHECK_RESULT_FALSE(CreateBoneNodeTable(_Initializer));

		return true;

	} while (0);

	Finalize();
	return false;
}

void CPmdSkeleton::Finalize()
{
	if (m_Table)
	{
		for (auto& elem : *m_Table)
		{
			EDENS_NAMESPACE::Delete(elem.second);
		}
		EDENS_NAMESPACE::Delete(m_Table);
	}

	EDENS_NAMESPACE::Delete(m_Matrices);
}

bool CPmdSkeleton::CreateBoneNodeTable(const FPmdSkeletonInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	uint32 boneNum = _Initializer->BoneNum;

	m_Matrices->resize(boneNum);

	for (uint32 index = 0; index < boneNum; ++index)
	{
		const auto& boneInfo = _Initializer->Bones[index];

		// ボーンノードのテーブルを作る

	}

	return false;
}
