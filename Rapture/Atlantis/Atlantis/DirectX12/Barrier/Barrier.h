/*****************************************************************//**
 * \file   Barrier.h
 * \brief  D3Dバリアクラスの定義
 *********************************************************************/
#pragma once

#include <Atlantis/DirectX12/DirectXPaste.h>

struct D3D12_RESOURCE_BARRIER;

ATLANTIS_NAMESPACE_BEGIN

/**
 * @brief D3Dバリアクラス
 */
class CBarrier
{
public:

	/**
	 * @brief バリア初期化用構造体
	 */
	struct FInitializer
	{
		/** バリアタイプ */
		Glue::EResourceBarrierType BarrierType = {};
	};

	/**
	 * @brief バリアステート構造体
	 */
	struct FTransitionState
	{
		/** ステートを変更するリソース */
		struct ID3D12Resource* Resource = nullptr;
		
		/** 変更前のステート */
		Glue::EResourceState Before = {};
		
		/** 変更後のステート */
		Glue::EResourceState After = {};
	};

	/**
	 * @brief バリア初期化関数
	 * 
	 * \param[in] _Initializer 初期化用構造体
	 * \return 初期化に成功したかどうか
	 */
	bool Initializer(const FInitializer& _Initializer);
	
	/**
	 * @brief バリア終了関数
	 * 
	 */
	void Finalize();

	/**
	 * @brief バリアステート設定関数
	 * 
	 * \param[in] _state バリアステート構造体
	 */
	void SetTransitionState(const FTransitionState& _state);

	/**
	 * @brief バリアステート変更関数
	 * 
	 */
	void SwapTransitionState();

	/**
	 * @brief バリア取得関数(DirectXAPI同士での連携用)
	 */
	D3D12_RESOURCE_BARRIER* GetBarrier() const { return m_Barrier; };

	CBarrier() {};
	~CBarrier() {};

private:

	/**
	 * @brief 生成初期化関数
	 * 
	 * \param[in] _Initializer 初期化用構造体
	 * \return 初期化に成功したかどうか
	 */
	bool CreateResourceBarrier(const FInitializer& _Initializer);

	/** バリア変数 */
	ObjectPtr(D3D12_RESOURCE_BARRIER) m_Barrier = nullptr;

};


ATLANTIS_NAMESPACE_END