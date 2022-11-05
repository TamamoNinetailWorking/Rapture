/////////////////////////////////////////////////////////////////////////////
///@file	ender_utility.h
///@brief	utilities when theris use finalize.
///@data	2020/06/25
////////////////////////////////////////////////////////////////////////////
#ifndef __ENDER_UTILITY_H__
#define __ENDER_UTILITY_H__

EDENS_NAMESPACE_BEGIN

template <class T>
inline void Delete(T*& p)
{
	if (p)
	{
		delete p;
	}
	p = nullptr;
}

template <class T>
inline void Release(T*& p)
{
	if (p)
	{
		p->Release();
	}
	p = nullptr;
}

template <class T>
inline void Exit(T*& p)
{
	if (p)
	{
		p->Exit();
	}
	p = nullptr;
}

template <class T>
inline void FinalizePtr(T& p)
{
	if (p)
	{
		p->Finalize();
		p.reset(nullptr);
	}
}

template <class T>
inline void ResetPtr(T& p)
{
	if (p)
	{
		p.reset(nullptr);
	}
}

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

template <class T>
inline void DeleteArray(T*& p)
{
	if (p)
	{
		delete[] p;
	}
	p = nullptr;
}


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
