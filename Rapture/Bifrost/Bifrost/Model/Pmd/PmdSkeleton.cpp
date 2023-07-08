#include "PmdSkeleton.h"
#include "PmdSkeletonInitializer.h"
#include "PmdPreDefine.h"
#include "PmdSkeletonDefine.h"

#include <eden/include/utility/ender_utility.h>

#include <DirectXMath.h>

//
// 
// Skeleton�ň�ɂ܂Ƃ߂Ă��邪�E�E�E
// >> ���Ƃ̃��O�f�[�^�����ʂ̏ꍇ�Ɏg���񂹂�悤�ɕ���������
// >> UE5���Q�l�ɂ���
// 
// PmdRig
// >> ���C���̃��O�f�[�^�i�X�P���g���f�[�^�j
// >> ���\�[�X�f�[�^�Ƃ��Ď���
// 
// PmdAnimationData
// >> VMD�t�@�C������ǂݍ��񂾃A�j���[�V�����f�[�^
// >> ���\�[�X�f�[�^�Ƃ��Ď���
// 
// PmdAnimationDrive
// >> AnimationData��Rig���Q�Ƃ��āA���ۂɉ�]���𐶐�����
// >> ���f���f�[�^�P�ʂŎ���
// >> Before��After�A������Current�̏�񂪕K�v�ɂȂ�͂�
// 
// PmdAnimationState
// >> �A�j���[�V�������m�̌q������X�e�[�g�}�V���ŕێ����A��������
// >> UE5��AnimationBP���C���[�W
// >> AnimationBP�͎g���܂킵�O��Ȃ̂ŁA��������\�[�X�f�[�^���낤
//
// 
// �������A�j���[�V�����f�[�^�����ۂɎ�舵����
// �v���O���~���O����̂͏��߂ĂȂ̂ŁA
// �܂��͖������̏͂��I���ӂ�܂ł͈�x�ώG�ɂȂ��Ă��g�ݍ���ł݂�ׂ����낤
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


#if 0
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

		FPmdBoneNode* node = (*m_Table)[CHash160("���r")];
		Vector3 pos = node->StartPos;

		FMatrix mat = XMMatrixTranslation(-pos.x, -pos.y, -pos.z) * XMMatrixRotationZ(XM_PIDIV2) * XMMatrixTranslation(pos.x, pos.y, pos.z);

		RecursiveMatrixMultiply(node, mat);

#endif

		
#ifdef VMD_SKELETON_TEST // VMD�t�@�C���K���e�X�g

		//Hash160 fileName = CHash160("resource/mmd/UserFile/Motion/pose.vmd");
		//Hash160 fileName = CHash160("resource/mmd/UserFile/Motion/swing2.vmd");
		Hash160 fileName = CHash160("resource/mmd/UserFile/Motion/motion.vmd");
		//Hash160 fileName = CHash160("resource/mmd/UserFile/Motion/TellYourWorld.vmd");

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
		initializer.EndFrameNo = m_VmdParser->GetMaxFrameNo();

		CMotionResourceManager* manager = PCast<CMotionResourceManager*>(CSubsystemServiceLocator::GetResourceSubsystemEdit()->GetMotionResourceManagerEdit());
		CHECK_RESULT_BREAK(manager);

		MotionHandle = manager->SearchCreateResource(&initializer);

		CHECK_RESULT_BREAK(manager->IsValidHandle(MotionHandle));

		const CVmdMotionResource* res = PCast<const CVmdMotionResource*>(manager->SearchResource(MotionHandle));

		PRINT("VmdMotionResource is Created.\n");

#if 0

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

		FPmdBoneNode* node = (*m_Table)[CHash160("�Z���^�[")];
		RecursiveMatrixMultiply(node, XMMatrixIdentity());

		PRINT("VmdMotionResource reflects.\n");
#endif

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

#ifdef VMD_SKELETON_TEST

bool IsPlayAnimation = false;
float AnimationTime = 0.f;
bool IsPlayLoop = false;

void CPmdSkeleton::PlayAnimation()
{
	// �A�j���[�V���������S�́A�A�j���[�V�����N���X���쐬����̂��Ó����낤
	// Skeleton�N���X�̓x�[�X�̃��O�f�[�^������ێ����A
	// �A�j���[�V�����f�[�^��������v�Z����`
	IsPlayLoop = true;
	IsPlayAnimation = true;
	AnimationTime = 0.f;
}

void CPmdSkeleton::MotionUpdate(float _DeltaTime)
{
	if (!IsPlayAnimation) { return; }

	MatrixReset();

	// �⊮����������
	//float frameTime = 30 * (AnimationTime / 1000.f);
	float frameTime = AnimationTime;
	uint32 frameNo = UNumCast(30 * frameTime);
	//PRINT("AnimationFrameTime %f,  FrameNo %d\n", frameTime, frameNo);

	CMotionResourceManager* manager = PCast<CMotionResourceManager*>(CSubsystemServiceLocator::GetResourceSubsystemEdit()->GetMotionResourceManagerEdit());
	CHECK(manager);
	CHECK(manager->IsValidHandle(MotionHandle));

	const CVmdMotionResource* res = PCast<const CVmdMotionResource*>(manager->SearchResource(MotionHandle));

	for (auto& bone : *m_Table)
	{
		const FVmdMotionPerKeyFrame* PrevMotion = res->FindCurrentMotionDataPerFrame(bone.first, frameNo);
		if (PrevMotion == nullptr) { continue; }

		FVector quat = {};

		const FVmdMotionPerKeyFrame* LastMotion = res->FindNextMotionDataPerFrame(bone.first, frameNo);
		if ((LastMotion == PrevMotion) || (LastMotion == nullptr))
		{
			quat = PrevMotion->Quaternion;
		}
		else
		{
			auto GetYFromXOnBezier = [](float _X,const Glue::Vector2& _P1,const Glue::Vector2& _P2,uint8 _N)
			{
				if (_P1.x == _P1.y && _P2.x == _P2.y)
				{
					return _X;
				}

				float t = _X;
				const float c0 = 1 + 3 * _P1.x - 3 * _P2.x;
				const float c1 = 3 * _P2.x - 6 * _P1.x;
				const float c2 = 3 * _P1.x;

				// EPSILON������������Ə������Ԃ������Ȃ��Ă��܂��̂ŁA
				// ����̏ꍇ�͓K���Ȓl�Ő؂��Ă����������S���낤
				//constexpr float epsilon = FLT_EPSILON;
				constexpr float epsilon = 0.0005f;

				for (uint32 i = 0; i < _N; ++i)
				{
					float ft = c0 * t * t * t + c1 * t * t + c2 * t - _X;

					if ((ft <= epsilon) && (ft >= -epsilon))
					{
						break;
					}

					t -= ft * 0.5f;
				}

				float r = 1 - t;

				float Y = t * t * t + 3 * t * t * r * _P2.y + 3 * t * r * _P1.x;

				return _X;
			};

			float rate = SCast<float>((frameNo - PrevMotion->FrameNo)) / (LastMotion->FrameNo - PrevMotion->FrameNo);

			// �x�W�F�Ȑ��ɂ��C�[�W���O���
			rate = GetYFromXOnBezier(rate, PrevMotion->BezierControlPoint01, PrevMotion->BezierControlPoint02, 12);

			quat = XMQuaternionSlerp(PrevMotion->Quaternion, LastMotion->Quaternion, rate);
			
			//PRINT("AnimationTime %f,Interpolate. Rate%f CurrentFrame %.3fPrevKey%u,LastKey%u \n",AnimationTime,rate,frameTime,PrevMotion->FrameNo,LastMotion->FrameNo);
		}

		auto& pos = bone.second->StartPos;
		FMatrix mat =
			XMMatrixTranslation(-pos.x, -pos.y, -pos.z)
			* XMMatrixRotationQuaternion(quat)
			* XMMatrixTranslation(pos.x, pos.y, pos.z);

		(*m_Matrices)[bone.second->BoneIndex] = mat;
	}

	AnimationTime += _DeltaTime;

	if (frameNo >= res->GetEndFrameNo())
	{
		if (!IsPlayLoop)
		{
			IsPlayAnimation = false;
		}

		AnimationTime = 0;
	}

	MatrixUpdate();
}

bool CPmdSkeleton::IsPlay()
{
	return IsPlayAnimation;
}

void CPmdSkeleton::MatrixReset()
{
	std::fill(
		m_Matrices->begin(),
		m_Matrices->end(),
		XMMatrixIdentity()
	);
}

void CPmdSkeleton::MatrixUpdate()
{
	std::function<void(FPmdBoneNode* _Node, const FMatrix& _Mat)> RecursiveMatrixMultiply = [&](FPmdBoneNode* _Node, const FMatrix& _Mat)
	{
		auto& ret = (*m_Matrices)[_Node->BoneIndex];
		ret *= _Mat;

		for (auto& child : _Node->children)
		{
			RecursiveMatrixMultiply(child, ret);
		}
	};

	FPmdBoneNode* node = (*m_Table)[CHash160("�Z���^�[")];
	RecursiveMatrixMultiply(node, XMMatrixIdentity());
}

#endif

bool CPmdSkeleton::CreateBoneNodeTable(const FPmdSkeletonInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	uint32 boneNum = _Initializer->BoneNum;

	const FPmdBoneData* bones = _Initializer->Bones;

	m_Matrices->resize(boneNum);

	for (uint32 index = 0; index < boneNum; ++index)
	{
		const FPmdBoneData& boneInfo = bones[index];

		// �{�[���m�[�h�̃e�[�u�������
		// >> ��x�e�[�u�������Ȃ��Ɛe�q�֌W���\�z�ł��Ȃ�

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
