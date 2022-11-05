#pragma once

ATLANTIS_NAMESPACE_BEGIN

// HRESULT�^��bool�^�ɕϊ����邾��
inline bool CheckD3DProcessResult(HRESULT _result)
{
	if (FAILED(_result))
	{
		return false;
	}
	else
	{
		return true;
	}
}


#define D3D_PROCESS_CHECK(result) CheckD3DProcessResult(result)

// �G���[�`�F�b�N���s���A���s���Ă���ꍇ�̓��^�[���܂ōs��
#define D3D_ERROR_CHECK(result) CHECK_RESULT_FALSE(D3D_PROCESS_CHECK(result))

// �|�C���^�`�F�b�N���A�ʏ�return����
#define D3D_CHECK(ptr) CHECK(ptr)


// D3D�֘A�̂��̂�unique_ptr����Release����Ƃ�
template <class T>
inline void ReleaseD3DPtr(T& _Ptr)
{
	if (_Ptr)
	{
		_Ptr->Release();
		_Ptr.release();
	}
}

// D3D�֘A�̂��̂�ʏ탊���[�X�����
template <class T>
inline void SafeReleaseD3DPtr(T& _Ptr)
{
	if (_Ptr)
	{
		_Ptr->Release();
	}
}


ATLANTIS_NAMESPACE_END
