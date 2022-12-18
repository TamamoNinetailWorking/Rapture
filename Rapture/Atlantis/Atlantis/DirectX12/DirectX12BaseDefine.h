#pragma once

ATLANTIS_NAMESPACE_BEGIN

// HRESULT�^��bool�^�ɕϊ����邾��
inline bool CheckD3DProcessResult(HRESULT _result)
{
	if (FAILED(_result))
	{
		PRINT("Error Num %u\n", _result);
		return false;
	}
	else
	{
		return true;
	}
}


#define D3D_PROCESS_CHECK(result) \
	CheckD3DProcessResult(result)	\

// �G���[�`�F�b�N���s���A���s���Ă���ꍇ��break����i���������z��j
#define D3D_INIT_PROCESS_CHECK(result) \
	if(!result) { break; } \

// �G���[�`�F�b�N���s���A���s���Ă���ꍇ�̓��^�[���܂ōs��
#define D3D_ERROR_CHECK(result) \
	CHECK_RESULT_FALSE(D3D_PROCESS_CHECK(result))\

// �|�C���^�`�F�b�N���A�ʏ�return����
#define D3D_CHECK(ptr) CHECK(ptr)
// �|�C���^�`�F�b�N���Anullptr�Ȃ�false��Ԃ�
#define D3D_CHECK_NULL(_ptr) if(_ptr == nullptr) { return false; };

// D3D�֘A�̂��̂�unique_ptr����Release����Ƃ�
template <class T>
inline void ReleaseD3DUniquePtr(T& _Ptr)
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
		_Ptr = nullptr;
	}
}


ATLANTIS_NAMESPACE_END
