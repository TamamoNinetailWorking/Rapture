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


#define D3D_PROCESS_CHECK(result) !CheckD3DProcessResult(result)

// �G���[�`�F�b�N���s���A���s���Ă���ꍇ�̓��^�[���܂ōs��
#define D3D_ERROR_CHECK(result) if(D3D_PROCESS_CHECK(result)) { return false; };


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


ATLANTIS_NAMESPACE_END
