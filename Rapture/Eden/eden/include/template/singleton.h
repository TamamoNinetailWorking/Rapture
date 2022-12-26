/////////////////////////////////////////////////////////////////////////////
///@file	singleton.h
///@brief	singleton template
///@data	2020/06/25
////////////////////////////////////////////////////////////////////////////
#ifndef __SINGLETON_H__
#define __SINGLETON_H__

//include header
#include <memory>

EDENS_NAMESPACE_BEGIN

template <class T>
class CSingleton
{
public:

	virtual ~CSingleton() {};

	static T& GetInstance()
	{
		static typename T::singleton_pointer_type s_singleton(T::createInstance());
		return getReference(s_singleton);
	}

protected:

	CSingleton() {};

private:

	typedef std::unique_ptr<T> singleton_pointer_type;

	CSingleton(const CSingleton&) = delete;
	CSingleton& operator=(const CSingleton&) = delete;
	CSingleton(CSingleton&&) = delete;
	CSingleton& operator=(CSingleton&&) = delete;

	inline static T* createInstance() {
		return new T();
	}
	inline static T& getReference(const singleton_pointer_type &ptr){
		return *ptr;
	}

};

EDENS_NAMESPACE_END

#endif