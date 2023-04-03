#include "PmdSkeleton.h"
#include "PmdSkeletonInitializer.h"
#include "PmdPreDefine.h"
#include "PmdSkeletonDefine.h"

#include <eden/include/utility/ender_utility.h>

#include <DirectXMath.h>

//
// 
// Skeletonで一つにまとめているが・・・
// >> もとのリグデータが共通の場合に使い回せるように分離したい
// >> UE5を参考にして
// 
// PmdRig
// >> メインのリグデータ（スケルトンデータ）
// >> リソースデータとして持つ
// 
// PmdAnimationData
// >> VMDファイルから読み込んだアニメーションデータ
// >> リソースデータとして持つ
// 
// PmdAnimationDrive
// >> AnimationDataとRigを参照して、実際に回転情報を生成する
// >> モデルデータ単位で持つ
// >> BeforeとAfter、そしてCurrentの情報が必要になるはず
// 
// PmdAnimationState
// >> アニメーション同士の繋がりをステートマシンで保持し、処理する
// >> UE5のAnimationBPをイメージ
// >> AnimationBPは使いまわし前提なので、これもリソースデータだろう
//
// 
// ※ただアニメーションデータを実際に取り扱って
// プログラミングするのは初めてなので、
// まずは魔導書の章が終わる辺りまでは一度煩雑になっても組み込んでみるべきだろう
//

#ifdef VMD_SKELETON_TEST
#include <eden/include/gadget/FileLoader/FileLoader.h>
#include <Bifrost/Model/Vmd/VmdParser.h>
#include <Bifrost/Resource/Motion/VmdMotionResourceInitializer.h>
#include <Bifrost/Resource/Motion/VmdMotionResource.h>
#include <Bifrost/Resource/Manager/MotionResourceManager.h>
#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Resource/ResourceSubsystemInterface.h>

#include <Bifrost/Model/Vmd/VmdMotionDefine.h>
#endif


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

#ifdef VMD_SKELETON_TEST
EDENS_NAMESPACE::FResourceHandle MotionHandle = {};
#endif

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


#ifdef VMD_SKELETON_TEST
		std::function<void(FPmdBoneNode* _Node, const FMatrix& _Mat)> RecursiveMatrixMultiply = [&](FPmdBoneNode* _Node, const FMatrix& _Mat)
		{
			auto& ret = (*m_Matrices)[_Node->BoneIndex];
			ret *= _Mat;

			for (auto& child : _Node->children)
			{
				RecursiveMatrixMultiply(child, ret);
			}
		};
#endif

#if 0

		FPmdBoneNode* node = (*m_Table)[CHash160("左腕")];
		Vector3 pos = node->StartPos;

		FMatrix mat = XMMatrixTranslation(-pos.x, -pos.y, -pos.z) * XMMatrixRotationZ(XM_PIDIV2) * XMMatrixTranslation(pos.x, pos.y, pos.z);

		RecursiveMatrixMultiply(node, mat);

#endif

		// VMDファイル適応テスト
#if 1
//#ifdef VMD_SKELETON_TEST

		Hash160 fileName = CHash160("resource/mmd/UserFile/Motion/pose.vmd");

		EDENS_NAMESPACE::CFileLoader loader = {};
		bool result = loader.FileLoad(fileName);

		if (result)
		{
			PRINT("VmdFile load Succeed\n");

			m_VmdParser = new CVmdParser();
			result = m_VmdParser->ParseData(loader.GetData(), loader.GetSize());
			if (result)
			{
				PRINT("VmdParse Success.\n");
			}
			else
			{
				PRINT("VmdParse Failed.\n");
			}
		}
		else
		{
			PRINT("VmdFile not loaded.\n");
		}

		loader.ResetData();

		FVmdMotionResourceInitializer initializer = {};
		initializer.Name = fileName;
		initializer.Type = EMotionType::MOTION_TYPE_VMD;
		initializer.Motions = m_VmdParser->GetMotionData();
		initializer.MotionNum = m_VmdParser->GetMotionDataNum();

		CMotionResourceManager* manager = PCast<CMotionResourceManager*>(CSubsystemServiceLocator::GetResourceSubsystemEdit()->GetMotionResourceManagerEdit());
		CHECK_RESULT_BREAK(manager);

		MotionHandle = manager->SearchCreateResource(&initializer);

		CHECK_RESULT_BREAK(manager->IsValidHandle(MotionHandle));

		const CVmdMotionResource* res = PCast<const CVmdMotionResource*>(manager->SearchResource(MotionHandle));

		PRINT("VmdMotionResource is Created.\n");

		for (auto& bone : *m_Table)
		{
			const FVmdMotionPerKeyFrame* data = res->FindMotionDataPerFrame(bone.first, 0);
			if (data == nullptr) { continue; }

			auto& pos = bone.second->StartPos;
			FMatrix mat = 
				XMMatrixTranslation(-pos.x,-pos.y,-pos.z) 
				* XMMatrixRotationQuaternion(data->Quaternion) 
				* XMMatrixTranslation(pos.x,pos.y,pos.z);

			(*m_Matrices)[bone.second->BoneIndex] = mat;
		}

		FPmdBoneNode* node = (*m_Table)[CHash160("センター")];

		RecursiveMatrixMultiply(node, XMMatrixIdentity());

		PRINT("VmdMotionResource reflects.\n");

#endif

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

const CPmdSkeleton::FBoneMatrices* CPmdSkeleton::GetBoneMatrices() const
{
	return m_Matrices;
}

uint32 CPmdSkeleton::GetBoneMatricesNum() const
{
	if (m_Matrices == nullptr) { return 0; }
	return SCast<uint32>(m_Matrices->size());
}

uint32 CPmdSkeleton::GetBoneMatricesSize() const
{
	return GetBoneMatricesNum() * sizeof(ATLANTIS_NAMESPACE::Glue::FMatrix);
}

bool CPmdSkeleton::CreateBoneNodeTable(const FPmdSkeletonInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	uint32 boneNum = _Initializer->BoneNum;

	const FPmdBoneData* bones = _Initializer->Bones;

	m_Matrices->resize(boneNum);

	for (uint32 index = 0; index < boneNum; ++index)
	{
		const FPmdBoneData& boneInfo = bones[index];

		// ボーンノードのテーブルを作る
		// >> 一度テーブルを作らないと親子関係も構築できない

		Hash160 name = CHash160(boneInfo.BoneName);

		(*m_Table)[name] = new FPmdBoneNode();
		(*m_Table)[name]->BoneIndex = index;
		(*m_Table)[name]->StartPos = boneInfo.BasePos;
	}

	for (uint32 index = 0; index < boneNum; ++index)
	{
		const FPmdBoneData& boneInfo = bones[index];
		
		if (boneInfo.ParentNo >= boneNum)
		{
			continue;
		}

		const FPmdBoneData& parentData = bones[boneInfo.ParentNo];

		Hash160 parentName = CHash160(parentData.BoneName);
		Hash160 boneName = CHash160(boneInfo.BoneName);

		(*m_Table)[parentName]->children.emplace_back((*m_Table)[boneName]);
	}

	std::fill(
		m_Matrices->begin(),
		m_Matrices->end(),
		XMMatrixIdentity()
	);

	PRINT("PmdSkeleton ready.\n");

	return true;
}
