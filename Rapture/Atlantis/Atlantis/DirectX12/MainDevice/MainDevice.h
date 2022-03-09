#pragma once

#include <memory>
#include <eden/include/template/NonCopyable.h>

#include <d3d12.h>

struct ID3D12Device;
struct IDXGIFactory6;
struct IDXGIAdapter;

ATLANTIS_NAMESPACE_BEGIN

/*
* DirectX12の基幹クラス
*/
class CDX12MainDevice : public CNonCopyable
{
public:

	// 初期化
	bool Initialize();

	// 終了処理
	void Finalize();

	// デバッグレイヤーの有効化
	bool EnableDebugLayer() const;


	// ゲッター
	ID3D12Device* GetDevice() const { return m_Device.get(); };
	IDXGIFactory6* GetGIFactory() const { return m_GIFactory.get(); };

	CDX12MainDevice() {};
	~CDX12MainDevice() {};

private:

	// GIFactoryの生成
	bool CreateGIFacotry();

	// DirectXデバイスの生成
	bool CreateDevice();

	// グラフィックアダプターの取得
	bool GetAdapter();


	// メインになるDirectXデバイス
	std::unique_ptr<ID3D12Device> m_Device = nullptr;

	// GIFactory
	std::unique_ptr<IDXGIFactory6> m_GIFactory = nullptr;

	// グラフィックアダプター
	std::unique_ptr<IDXGIAdapter> m_Adapter = nullptr;

	// フィーチャーレベル
	D3D_FEATURE_LEVEL m_FeatureLevel = {};

};


ATLANTIS_NAMESPACE_END