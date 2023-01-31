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


#include <Atlantis/Material/MaterialInterface.h>

ATLANTIS_NAMESPACE_BEGIN

struct FMaterialInterfaceInitializerBase;
class CGraphicsPipeline;
class CRootSignature;

struct FMaterialGeometryBufferBase;

ATLANTIS_NAMESPACE_END

BIFROST_NAMESPACE_BEGIN

class CPmdMaterialData : public ATLANTIS_NAMESPACE::IMaterialInterface
{
public:

	bool Initialize(const ATLANTIS_NAMESPACE::FMaterialInterfaceInitializerBase* _Initializer) override;
	void Finalize() override;

    void SetGeometryBuffer(ATLANTIS_NAMESPACE::FMaterialGeometryBufferBase* _Buffer) override;

	uint32 GetDrawIndex(uint32 _Index) const override;
	uint32 GetMaterialNum() const override;

    ATLANTIS_NAMESPACE::CGraphicsPipeline* GetGraphicsPipeline() const override;
    ATLANTIS_NAMESPACE::CRootSignature* GetRootSignature() const override;

    const ATLANTIS_NAMESPACE::FMaterialGeometryBufferBase* GetGeometryBuffer() const override;
    ATLANTIS_NAMESPACE::FMaterialGeometryBufferBase* GetGeometryBufferEdit() const override;

	CPmdMaterialData();
	~CPmdMaterialData();

private:

	class Impl;
	ObjectPtr(Impl) m_Impl = nullptr;
};

BIFROST_NAMESPACE_END