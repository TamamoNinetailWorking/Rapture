#pragma once

#include <Atlantis/DirectX12/DirectXPaste.h>

ATLANTIS_NAMESPACE_BEGIN

class CDX12MainDevice;
class CCommandQueue;

struct FRHIRenderTargetInitializer
{
	const CDX12MainDevice* Device = nullptr;
	const CCommandQueue* CommandQueue = nullptr;

	uint32 ViewWidth = 0;
	uint32 ViewHeight = 0;
	HWND WindowHandle = {};

	Glue::EDataFormat Format = {};
	Glue::ERTVDimension RTVDimension = {};

};

ATLANTIS_NAMESPACE_END