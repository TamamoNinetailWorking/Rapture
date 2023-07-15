/*****************************************************************//**
 * \file   ResourceManagerPreDefine.h
 * \brief  リソースの管理クラスの前方宣言
 *********************************************************************/
#pragma once

/*
*    　>>Handleは別のヘッダーに定義し直したほうがいいかもしれない。Handleだけで持つときにオーバーヘッドが大きくなる
      >>ResourceクラスのInitializerも似た理由で別定義したほうが良さそう
	  


	  >>こいつListのIteratorで持ってるから別Header化できない
		Handleの形を改めて考え直したほうがいいかもしれない
* 
*/

#include <list>

EDENS_NAMESPACE_BEGIN

/** リソースの基底クラス  */
class CResource;

/**
 * @brief リソースの管理クラスで実際に管理するリソースの構造体
 */
struct FManagementResource
{
private:
	friend class CResourceManager;
	
	/** リソースのポインタ */
	ObjectPtr(CResource) ResPtr = nullptr;

	/** 参照カウンタ */
	sint32 RefCount = 0;
};


// ハンドルで直接iteratorを持つのは比較処理が面倒になる
// 有効・無効判定が簡単には行えない
// >> iteratorが空の状態で比較しようとするとexceptionが飛んでくる

/** リソースリスト */
typedef std::list<FManagementResource> FResourceList;

/**
 * @brief リソースの管理用ハンドル
 */
struct FResourceHandle
{
private:
	friend class CResourceManager;

	typedef FResourceList::iterator Handle;

	/** ハンドルで取り扱うリストへのアクセッサー */
	Handle iterator = {};

	/** ハンドルが有効かどうか */
	bool isActive = false;

public:

	/**
	 * @brief ハンドルが同じかどうか
	 * 
	 * \param[in] _handle 比較するハンドル
	 * \return ハンドルが同じかどうか
	 */
	bool operator==(const FResourceHandle& _handle) const
	{
		return iterator == _handle.iterator;
	}

	/**
	 * @brief ハンドルが違うかどうか
	 * 
	 * \param[in] _handle 比較するハンドル
	 * \return ハンドルが違うかどうか
	 */
	bool operator !=(const FResourceHandle& _handle) const
	{
		return *this != _handle;
	}
};

static const FResourceHandle INVALID_RESOURCE_HANDLE = FResourceHandle();

EDENS_NAMESPACE_END