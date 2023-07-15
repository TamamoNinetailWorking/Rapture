/////////////////////////////////////////////////////////////////////////////
///@file	ender_utility.h
///@brief	utilities when theris use finalize.
///@data	2020/06/25
////////////////////////////////////////////////////////////////////////////
#ifndef __ENDER_UTILITY_H__
#define __ENDER_UTILITY_H__

EDENS_NAMESPACE_BEGIN

/**
 * @brief ポインタの削除とダングリング化防止
 * 
 * \param[in] p 削除するポインタ
 */
template <class T>
inline void Delete(T*& p)
{
	if (p)
	{
		delete p;
	}
	p = nullptr;
}

/**
 * @brief ポインタの解放とダングリング化防止
 *
 * \param[in] p 解放するポインタ
 * @details DirectX12オブジェクトなどで使用する
 */
template <class T>
inline void Release(T*& p)
{
	if (p)
	{
		p->Release();
	}
	p = nullptr;
}

/**
 * @brief　終了処理とダングリング化防止
 * 
 * \param[in] p 終了処理するポインタ
 */
template <class T>
inline void Exit(T*& p)
{
	if (p)
	{
		p->Exit();
	}
	p = nullptr;
}

/**
 * @brief 終了処理とユニークポインタのリセット
 * 
 * \param[in] p 終了処理するユニークポインタ
 */
template <class T>
inline void FinalizePtr(T& p)
{
	if (p)
	{
		p->Finalize();
		p.reset(nullptr);
	}
}

/**
 * @brief ユニークポインタのリセット
 *
 * \param[in] p ユニークポインタ
 */
template <class T>
inline void ResetPtr(T& p)
{
	if (p)
	{
		p.reset(nullptr);
	}
}

/**
 * @brief　終了処理とダングリング化防止
 *
 * \param[in] p 終了処理するポインタ
 */
template <class T>
inline void FinalizeObject(T& p)
{
	if (p)
	{
		p->Finalize();
		delete p;
	}
	p = nullptr;
}

/**
 * @brief　配列の削除とダングリング化防止
 *
 * \param[in] p 削除する配列ポインタ
 */
template <class T>
inline void DeleteArray(T*& p)
{
	if (p)
	{
		delete[] p;
	}
	p = nullptr;
}

/**
 * @brief　解放の削除とダングリング化防止
 *
 * \param[in] p 解放する配列ポインタ
 */
template <class T>
inline void ReleasePtr(T& _Ptr)
{
	if (_Ptr)
	{
		_Ptr->Release();
		_Ptr.release();
	}
}

#if 0
#include <list>
#include <vector>

template <class T>
void DeleteListElem(std::list<T*>& arr, T* elem)
{
	typename std::list<T*>::iterator itr;
	for (itr = arr.begin(); itr != arr.end();)
	{
		if ((*itr) == elem)
		{
			Delete(*itr);
			itr = arr.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}

template <class T>
void DeleteList(std::list<T*>& arr)
{
	typename std::list<T*>::iterator itr;
	for (itr = arr.begin(); itr != arr.end(); itr++)
	{
		Delete(*itr);
	}
	arr.clear();
}

template <class T>
void ReleaseVector(std::vector<T*>& arr)
{
	for (auto itr = arr.begin(); itr != arr.end(); itr++)
	{
		Release(*itr);
	}
	arr.clear();
}
#endif

EDENS_NAMESPACE_END

#endif
