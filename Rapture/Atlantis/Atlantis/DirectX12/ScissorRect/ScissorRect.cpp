#include "ScissorRect.h"

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>

USING_ATLANTIS;

bool CScissorRect::Initialize(const FInitializer& _Initializer)
{
	do
	{
		if (!CreateScissorRect(_Initializer)) { break; }

		return true;

	} while (0);

	return false;
}

void CScissorRect::Finalize()
{
	if (m_ScissorRect)
	{
		//m_ScissorRect.reset();
		delete m_ScissorRect;
	}
}

bool CScissorRect::CreateScissorRect(const FInitializer& _Initializer)
{
	D3D12_RECT* rect = new D3D12_RECT();
	if (!rect) { return false; };

	rect->top = 0;
	rect->left = 0;
	rect->right = rect->left + _Initializer.RectWidth;
	rect->bottom = rect->top + _Initializer.RectHeight;

	//m_ScissorRect.reset(rect);
	m_ScissorRect = rect;

	return true;
}
