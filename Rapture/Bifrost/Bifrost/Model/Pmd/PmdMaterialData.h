#pragma once

/*
*     PmdMaterialData���쐬����
     >>�}�e���A���f�[�^���V���A���C�U�[����z���グ��HLSL�ɑ���o���f�[�^���܂Ƃ߂ĕێ�����
       >>Pmd���f����`�悷��ۂɂ͕K�{
     >>TextureData�������Ă���
        >>�t���C�E�F�C�g�p�^�[���I�ɁA��x�쐬�������\�[�X���č쐬�������Ȃ�
            >>����toonXX.bmp����
        >> �܂肱�̃^�C�~���O��Texture��ID3D12Resource���p�b�L���O�����N���X�ƁA
            Texture�̊Ǘ����s��Subsystem���K�v�ɂȂ�
            >> Subsystem�̋N�����ɔ��e�N�X�`���⍕�e�N�X�`�������悤�ɂ��Ă���
                >> ��邽�߂̃N���X�I�u�W�F�N�g��Atlantis���Œ�`���Ă����Ƃ������낤
*/

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

	ID3D12DescriptorHeap* GetDescriptorHeap() const;
	ID3D12DescriptorHeap* const* GetDescriptorHeapPtr() const;
	uint32 GetHeapStride() const;
	uint32 GetDrawIndex(uint32 _Index) const;
	uint32 GetMaterialNum() const;

	CPmdMaterialData();
	~CPmdMaterialData();

private:

	class Impl;
	ObjectPtr(Impl) m_Impl = nullptr;
};

BIFROST_NAMESPACE_END