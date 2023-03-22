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

		(*m_Table)[parentName]->children.emplace_back((*m_Table)[parentName]);
	}

	std::fill(
		m_Matrices->begin(),
		m_Matrices->end(),
		XMMatrixIdentity()
	);

	PRINT("PmdSkeleton ready.\n");

	return true;
}
