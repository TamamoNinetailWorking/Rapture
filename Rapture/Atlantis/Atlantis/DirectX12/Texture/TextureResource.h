#pragma once

/*
*    Texture��V�F�[�_�[��FileLoad����𕪗����āA
    Memory����f�[�^�쐬����API�ɍ����ւ��Ă����������B
    �@>>���͓��ɍ���Ȃ����A
    �@�@�v���b�g�t�H�[�����ς�����Ƃ��Ƀt�@�C���V�X�e���������ւ�����B
    GraphicsShader��
        >> �������Ȃ��Ƃ����Ȃ��̂�ShaderBase���ɋL�q��������
*/

#include <eden/include/Resource/Resource.h>

//struct ID3D12Device;
struct ID3D12Resource;

EDENS_NAMESPACE_BEGIN

struct FResourceInitializerBase;

EDENS_NAMESPACE_END

ATLANTIS_NAMESPACE_BEGIN

struct FTextureInitializer;

class CTextureResource : public EDENS_NAMESPACE::CResource
{
public:

	bool Initialize(const EDENS_NAMESPACE::FResourceInitializerBase* _initializer) override;
	void Finalize() override;

	ID3D12Resource* GetResource() const { return m_Resource; };

private:

	bool CreateTextureResource(const FTextureInitializer* _Initializer);
	bool CopyToVideoMemory(const FTextureInitializer* _Initializer);

	ObjectPtr(ID3D12Resource) m_Resource = nullptr;

};

ATLANTIS_NAMESPACE_END