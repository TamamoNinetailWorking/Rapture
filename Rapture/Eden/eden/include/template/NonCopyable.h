#pragma once

//�h���N���X���R�s�[�֎~�ɂ��邽�߂�
//������Z�q�̃I�[�o�[���[�h��private��
//	��Singleton�̊��N���X�������B
class CNonCopyable
{
protected://�p�����Ă����������g�p�\

	CNonCopyable() {};
	virtual ~CNonCopyable() {};

private:
	//��������ɓ�������̂�private�ɂ��Ďg�p�֎~��
	CNonCopyable(const CNonCopyable&);
	const CNonCopyable& operator=(const CNonCopyable&);
};