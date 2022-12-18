#pragma once

ATLANTIS_NAMESPACE_BEGIN

// HRESULT型をbool型に変換するだけ
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

// エラーチェックを行い、失敗している場合はbreakする（初期化時想定）
#define D3D_INIT_PROCESS_CHECK(result) \
	if(!result) { break; } \

// エラーチェックを行い、失敗している場合はリターンまで行う
#define D3D_ERROR_CHECK(result) \
	CHECK_RESULT_FALSE(D3D_PROCESS_CHECK(result))\

// ポインタチェックし、通常returnする
#define D3D_CHECK(ptr) CHECK(ptr)
// ポインタチェックし、nullptrならfalseを返す
#define D3D_CHECK_NULL(_ptr) if(_ptr == nullptr) { return false; };

// D3D関連のものをunique_ptrからReleaseするとき
template <class T>
inline void ReleaseD3DUniquePtr(T& _Ptr)
{
	if (_Ptr)
	{
		_Ptr->Release();
		_Ptr.release();
	}
}

// D3D関連のものを通常リリースする際
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
