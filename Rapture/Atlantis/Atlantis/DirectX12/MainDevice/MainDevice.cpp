#include "MainDevice.h"

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>

#include <vector>
#include <string>

#include <dxgi1_6.h>

using namespace std;

USING_ATLANTIS;

bool CDX12MainDevice::Initialize()
{
	do{

#ifdef ATLANTIS_DEBUG
		// デバッグレイヤーの有効化
		if (!EnableDebugLayer())
		{
			break;
		}
#endif

		if (!CreateGIFacotry())
		{
			break;
		}

		// アダプターの取得
		if (!GetAdapter()) { break; };

		if (!CreateDevice())
		{
			break;
		}

		// 初期化成功
		return true;

	} while (0);


	// 初期化失敗
	Finalize();

	return false;
}

void CDX12MainDevice::Finalize()
{

	ReleaseD3DPtr(m_Device);
	ReleaseD3DPtr(m_Adapter);
	ReleaseD3DPtr(m_GIFactory);
}

bool CDX12MainDevice::EnableDebugLayer() const
{
#ifdef ATLANTIS_DEBUG
	// デバッグレイヤーをONにする
	ID3D12Debug* debugLayer = nullptr;
	D3D_ERROR_CHECK(D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer)));
		debugLayer->EnableDebugLayer();

		// デバッグレイヤーを有効化した後はこのインターフェースは不要らしい
		debugLayer->Release();
#endif

	return true;
}

bool CDX12MainDevice::CreateGIFacotry()
{
	// GIFactoryの生成
	IDXGIFactory6* factory = nullptr;
#ifdef ATLANTIS_DEBUG
	D3D_ERROR_CHECK(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&factory)));
#else
	D3D_ERROR_CHECK(CreateDXGIFactory2(0, IID_PPV_ARGS(&factory)));
#endif

	// ユニークポインタ化
	m_GIFactory = move(unique_ptr<IDXGIFactory6>(factory));

	return true;
}

bool CDX12MainDevice::CreateDevice()
{
	// フィーチャーレベル列挙
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	ID3D12Device* device = nullptr;
	for (auto& elem : featureLevels)
	{
		if (D3D_PROCESS_CHECK(D3D12CreateDevice(m_Adapter.get(), elem, IID_PPV_ARGS(&device))))
		{
			m_FeatureLevel = elem;
			break;
		}
	}

	if (device == nullptr) { return false; }

	m_Device = move(unique_ptr<ID3D12Device>(device));
	return true;
}

bool CDX12MainDevice::GetAdapter()
{
	IDXGIAdapter* adapter = nullptr;
	vector<IDXGIAdapter*> adapters;
	IDXGIAdapter* tempAdapter = nullptr;

	for (uint32 i = 0; ; ++i)
	{
		if (m_GIFactory->EnumAdapters(i, &tempAdapter) == DXGI_ERROR_NOT_FOUND)
		{
			break;
		}

		adapters.push_back(tempAdapter);
	}

	for (auto& adpt : adapters)
	{
		DXGI_ADAPTER_DESC desc = {};
		adpt->GetDesc(&desc);

		wstring stringDesc = desc.Description;
		if (stringDesc.find(L"NVIDIA") != string::npos)
		{
			adapter = adpt;
			break;
		}
	}

	if (adapter == nullptr)
	{
		return false;
	}

	m_Adapter = move(unique_ptr<IDXGIAdapter>(adapter));
	return true;
}
