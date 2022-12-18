#include "DirectXDebug.h"
#include <d3d12.h>
#include <Atlantis/DirectX12/DirectX12BaseDefine.h>

using namespace ATLANTIS_NAMESPACE;

bool CDirectXDebug::EnableDebugLayer() const
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

CDirectXDebug::~CDirectXDebug()
{
}
