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

/**
 * @brief シングルトンクラス
 * @tparam シングルトンにしたいクラス
 */
template <class T>
class CSingleton
{
public:

	/**
	 * @brief デストラクタ
	 */
	virtual ~CSingleton() {};

	/**
	 * @brief シングルトンのインスタンスを取得
	 * 
	 * \return シングルトンへの参照
	 */
	static T& GetInstance()
	{
		static typename T::singleton_pointer_type s_singleton(T::createInstance());
		return getReference(s_singleton);
	}

protected:

	/**
	 * @brief コンストラクタ
	 * @details 他で生成されないようにするためにprotectedにしている
	 */
	CSingleton() {};

private:

	typedef std::unique_ptr<T> singleton_pointer_type;

	/**
	 * @brief コピーコンストラクタ
	 * @details 他で生成されないようにdelete
	 */
	CSingleton(const CSingleton&) = delete;

	/**
	 * @brief コピーコンストラクタ
	 * @details 他で生成されないようにdelete
	 */
	CSingleton& operator=(const CSingleton&) = delete;

	/**
	 * @brief コピーコンストラクタ
	 * @details 他で生成されないようにdelete
	 */
	CSingleton(CSingleton&&) = delete;

	/**
	 * @brief 代入演算子
	 * @details 他で生成されないようにdelete
	 */
	CSingleton& operator=(CSingleton&&) = delete;

	/**
	 * @brief インスタンスを生成する
	 */
	inline static T* createInstance() {
		return new T();
	}

	/**
	 * @brief インスタンスの参照を取得する
	 * 
	 * \param ptr
	 * \return 
	 */
	inline static T& getReference(const singleton_pointer_type &ptr){
		return *ptr;
	}

};

EDENS_NAMESPACE_END

#endif