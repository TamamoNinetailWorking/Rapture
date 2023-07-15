/*****************************************************************//**
 * \file   Resource.h
 * \brief  管理するリソースの基底クラス　定義
 *********************************************************************/
#pragma once

//#include "ResourceInitializer.h"

EDENS_NAMESPACE_BEGIN

/** リソースの初期化構造体の基底 */
struct FResourceInitializerBase;

/**
 * @brief　リソースの基底クラス
 */
class CResource
{
public:

	/**
	 * @brief リソースの初期化
	 * 
	 * \param[in] _Initializer リソースを初期化するための構造体（継承可能）
	 * \return 初期化に成功したかどうか
	 */
	virtual bool Initialize(const FResourceInitializerBase* _Initializer) = 0;
	
	/**
	 * @brief リソースの終了処理
	 * 
	 */
	virtual void Finalize() = 0;

	/**
	 * @brief リソースの名前を取得する
	 * @return リソースの名前
	 */
	const Hash160& GetName() const { return m_ResourceName; };

	/**
	 * @brief コンストラクタ
	 */
	CResource() {};

	/**
	 * @brief デストラクタ
	 * 
	 */
	virtual ~CResource();

protected:

	/** リソースの名前 */
	Hash160 m_ResourceName = {};

};


EDENS_NAMESPACE_END