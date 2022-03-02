#pragma once

EDENS_NAMESPACE_BEGIN

/*
*	GUI�E�B���h�E�����̂��߂̏������\����
*/
struct FWindowInitializer
{

	HINSTANCE Instance = nullptr;//�E�B���h�E�\����
	const c8* WindowName = nullptr;//�E�B���h�E��
	const c8* WindowTitle = nullptr;//�E�B���h�E�^�C�g��
	s32 WindowMode = SW_HIDE; // �E�B���h�E���[�h
	s32 WindowWidth = 0;//�E�B���h�E�̉���
	s32 WindowHeight = 0;//�E�B���h�E�̍���
	b8 IsFullScreen = false;//�t���X�N���[���ݒ�

};

EDENS_NAMESPACE_END