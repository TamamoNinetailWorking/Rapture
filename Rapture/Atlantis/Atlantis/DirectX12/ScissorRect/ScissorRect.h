#pragma once

#include <memory>

#include <d3d12.h>

ATLANTIS_NAMESPACE_BEGIN

class CScissorRect
{
public:

	struct FInitializer
	{
		uint32 RectWidth = 0;
		uint32 RectHeight = 0;
	};

	bool Initialize(const FInitializer& _Initializer);
	
	void Finalize();


	const D3D12_RECT* GetScissorRect() const { return m_ScissorRect; };


	CScissorRect() {};
	~CScissorRect() {};

private:

	bool CreateScissorRect(const FInitializer& _Initializer);


	//std::unique_ptr<D3D12_RECT> m_ScissorRect = nullptr;
	ObjectPtr(D3D12_RECT) m_ScissorRect = nullptr;

};


ATLANTIS_NAMESPACE_END