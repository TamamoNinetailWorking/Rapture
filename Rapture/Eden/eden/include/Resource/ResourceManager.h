/*****************************************************************//**
 * \file   ResourceManager.h
 * \brief  リソースの管理クラス定義
 *********************************************************************/
#pragma once

#include "ResourceManagerPreDefine.h"

EDENS_NAMESPACE_BEGIN

//class CResource;

/** リソースの初期化構造体  */
struct FResourceInitializerBase;


// Note 参照カウンタ周りの機能は改修が必要になりそう

/**
 * @brief リソースの管理クラス
 */
class CResourceManager
{
public:

	/**
	 * @brief マネージャの初期化
	 * 
	 * \return 初期化が成功したかどうか
	 */
	bool Initialize();

	/**
	 * @brief マネージャの終了処理
	 * 
	 */
	void Finalize();

	//template <class TResource>

	/**
	 * @brief リソースの検索と作成
	 * 
	 * \param[in] _Initializer リソースの初期化構造体
	 * \return 管理用のリソースハンドル
	 */
	FResourceHandle SearchCreateResource(const FResourceInitializerBase* _Initializer);
	
	/**
	 * @brief リソースの検索
	 * 
	 * \param[in] _Handle 管理用のリソースハンドル
	 * \return リソースのポインタ
	 */
	const CResource* SearchResource(const FResourceHandle& _Handle) const;

	/**
	 * @brief リソースの検索
	 * 
	 * \param[in] _Hash リソースのハッシュ値（名前）
	 * \return リソースのポインタ
	 */
	const CResource* SearchResource(const Hash160& _Hash);

	/**
	 * @brief リソースハンドルの検索
	 * 
	 * \param[in] _Hash リソースのハッシュ値（名前）
	 * \return リソースハンドル
	 */
	FResourceHandle SearchResourceHandle(const Hash160& _Hash);

	/**
	 * @brief リソースの検索
	 * 
	 * \param[in] _Handle リソースハンドル
	 * \return リソースのポインタ
	 * @details	リソースの参照カウンタを増やす
	 */
	const CResource* SearchResourceRef(const FResourceHandle& _Handle) const;

	/**
	* @brief リソースの参照カウンタを増やす
	* @param[in] _Handle リソースハンドル
	*/
	void AddRef(const FResourceHandle& _Handle) const;

	/**
	 * @brief リソースの削除
	 * 
	 * \param[in] _Handle リソースハンドル
	 * @details リソースの参照カウンタを減らし、0になったら削除する
	 */
	void DeleteResource(const FResourceHandle& _Handle);

	/**
	 * @brief リソースの削除
	 * 
	 * \param[in] _Hash リソースのハッシュ値（名前）
	 * @details リソースの参照カウンタを減らし、0になったら削除する
	 */
	void DeleteResource(const Hash160& _Hash);

	/**
	 * @brief リソースハンドルが有効かどうか
	 * 
	 * \param[in] _Handle リソースハンドル
	 * \return リソースハンドルが有効かどうか
	 */
	bool IsValidHandle(const FResourceHandle& _Handle) const;

	/**
	 * @brief リソースハンドルが無効かどうか
	 * 
	 * \param[in] _Handle リソースハンドル
	 * \return リソースハンドルが無効かどうか
	 */
	bool IsInvalidHandle(const FResourceHandle& _Handle) const;

	/**
	 * @brief リソースハンドルの無効値を取得
	 * 
	 * \return リソースハンドルの無効値
	 */
	FResourceHandle GetInvalidHandle() const;

	/**
	 * @brief コンストラクタ
	 */
	CResourceManager() {};

	/**
	 * @brief デストラクタ
	 */
	~CResourceManager() {};

protected:

	/**
	 * @brief リソースの作成処理
	 * 
	 * \param[in] _Initializer リソースの初期化構造体
	 * \return リソースハンドル
	 * @details リソースの作成に失敗した場合は無効なリソースハンドルを返す。
	 * @note リソース作成の実装処理は派生クラスで行う
	 */
	virtual FResourceHandle CreateResourceImpl(const FResourceInitializerBase* _Initializer) = 0;

	/**
	 * @brief リソースの作成処理
	 * @tparam TResource リソースクラス
	 * @param[in] _Initializer リソースの初期化構造体
	 * @return リソースハンドル
	 */
	template <class TResource>
	FResourceHandle CreateResource(const FResourceInitializerBase* _Initializer);

private:

	/**
	 * @brief リソースの検索
	 * 
	 * \param[in] _Initializer リソースの初期化構造体
	 * \return リソースハンドル
	 * @details inlファイル内での実装に使用する
	 */
	FResourceHandle SearchResourceHandle(const FResourceInitializerBase* _Initializer);

	/** 管理するリソースリスト */
	FResourceList m_ResourceList = {};

};


EDENS_NAMESPACE_END

#include "ResourceManager.inl"