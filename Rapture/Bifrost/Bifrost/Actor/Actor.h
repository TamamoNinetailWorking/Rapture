/*****************************************************************//**
 * \file   Actor.h
 * \brief  Actorクラスの定義
 ********************************************************************
 */
#pragma once

#include <vector>
#include <eden/include/Object/Object.h>
#include <Bifrost/Subsystem/Updater/UpdateGroupDefine.h>
#include <Bifrost/Subsystem/Updater/UpdateProcessData.h>

// Componentも含めてBifrost行きかな・・・
// サービスロケーターを継承式にして、最初に初期化するとき、一番根本のクラスで生成すればいけるのでは？
// ただ、Edenのライブラリとしての特色としてを残すのであれば、BifrostからActorやComponentを作ってもいい気がする
// やっぱりBifrostからかな

BIFROST_NAMESPACE_BEGIN

class CComponent;

struct FActorInitializerBase;

/**
* @brief	Actorクラス
* @details	レベル上にスポーンさせるオブジェクトの基底クラス
*/
class CActor : public EDENS_NAMESPACE::CObject 
{
public:

	/**
	*	@brief アクターの初期化
	*	@param[in] _Initializer アクターを初期化するための構造体（継承可能）
	*	@return 初期化に成功したかどうか
	*/
	virtual bool Initialize(const FActorInitializerBase* _Initializer = nullptr);

	/**
	*	@brief アクターの終了処理
	*/
	virtual void Finalize();

	/**
	*	@brief アクターにコンポーネントをアタッチする
	*	@param[in] _Component アタッチするコンポーネント
	*	@return アタッチに成功したかどうか
	*/
	bool AttachComponent(CComponent* _Component);

	/**
	*	@brief アクターからコンポーネントをデタッチする
	*	@param[in] _Component デタッチするコンポーネント
	*	@return デタッチに成功したかどうか
	*/
	bool DetachComponent(CComponent* _Component);

	/**
	*	@brief アクターにアタッチされたコンポーネントを検索する
	*	@tparam Component 検索するコンポーネントの型
	*	@return 検索に成功した場合はコンポーネントのポインタ、失敗した場合はnullptr
	*/
	template <class Component>
	Component* SearchComponent() const;

	/**
	*	@brief アクターの更新時初期化処理
	*	@details アクターがスポーンされた際に一度だけ呼ばれる
	*/
	virtual void BeginPlay();

	/**
	*	@brief アクターの更新時終了処理
	*	@details アクターが破棄される際に一度だけ呼ばれる
	*/
	virtual void EndPlay();

	/**
	*	@brief	アクターの削除予約
	*/
	void ReserveKill();

	/**
	*	@brief	アクターの更新処理
	*	@param[in]	_DeltaTime フレーム処理時間
	*	@return 更新処理が成功したかどうか
	*	@details 戻り値はデフォルトはfalse。継承先で更新処理を行う場合はtrueを返すようにする
	*/
	virtual bool Update(float _DeltaTime) { return false; };

	/**
	*	@brief	アクターのハッシュ値をセット
	*	@param[in]	_Hash ハッシュ値
	*/
	void SetHash(const Hash160& _Hash);

	/**
	*	@brief	アクターのハッシュ値を取得
	*	@return	ハッシュ値
	*/
	const Hash160& GetHash() const;

	/**
	*	@brief	アクターがアクティブかどうかを取得
	*	@return	アクティブかどうか
	*/
	bool IsActive() const;

	/**
	*	@brief	アクターがアクティブかどうかをセット
	*	@param[in]	_IsActive アクティブかどうか
	*/
	void SetActive(bool _IsActive);

	/**
	*	@brief	アクターが更新処理を行うかどうかを取得
	*	@return 更新処理を行うかどうか
	*/
	bool CanEverUpdate() const;

	/**
	* @brief コンストラクタ
	*/
	CActor();

	/**
	* @brief デストラクタ
	*/
	virtual ~CActor();

private:

	/** アクターのハッシュ値 */
	Hash160 m_ActorHash = {};

	using ComponentList = std::vector<CComponent*>;

	/** 保持するコンポーネントのリスト */
	ObjectPtr(ComponentList) m_ComponentList = nullptr;

	/** アクティブかどうか */
	bool m_IsActive = true;

	/** 恒久的に更新処理を行うかどうか */
	bool m_CanEverUpdate = false;

protected:

	/** 更新処理を行う更新グループの設定 */
	EUpdateGroup m_UpdateGroup = EUpdateGroup::UPDATE_GROUP_PRE_PHYSICS;

	/** 更新処理を行う更新グループに登録した際のハンドル */
	FUpdateProcessorHandle m_ProcessorHandle = {};

protected:

	/**
	 * @brief アクターのハッシュ値をセット
	 * 
	 * @param _Hash ハッシュ値
	 *
	 * @note 同様の関数が存在するので削除して良さそう
	 */
	void SetActorHash(const Hash160& _Hash);

	/**
	 * @brief コンポーネントリストの取得
	 * 
	 * \return コンポーネントリスト
	 * @details コンポーネントリスト変数はprivateなので、継承先でリストを取得するために使用する
	 */
	const ComponentList* GetComponentList() const;


};

BIFROST_NAMESPACE_END

#include "Actor.inl"