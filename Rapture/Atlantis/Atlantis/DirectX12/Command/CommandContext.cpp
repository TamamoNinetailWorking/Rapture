#include "CommandContext.h"

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>

using namespace std;

USING_ATLANTIS;
USING_GLUE;

bool CCommandContext::Initialize(const FCommandContextInitializer& _Initializer)
{
	do
	{
		if (!_Initializer.Device) { break; };

		if (!CreateCommandAllocator(_Initializer)) { break; }

		if (!CreateCommandList(_Initializer)) { break; }

		return true;

	} while (0);

	Finalize();

	return false;
}


void CCommandContext::Finalize()
{
	ReleaseD3DPtr(m_CmdList);
	ReleaseD3DPtr(m_CmdAllocator);
}


void CCommandContext::Reset(ID3D12PipelineState* _PipelineState)
{
	m_CmdAllocator->Reset();
	m_CmdList->Reset(m_CmdAllocator.get(), _PipelineState);
}


bool CCommandContext::CreateCommandAllocator(const FCommandContextInitializer& _Initializer)
{

	ID3D12CommandAllocator* allocator = nullptr;
	D3D_ERROR_CHECK(_Initializer.Device->CreateCommandAllocator(
		GetD3DCommandListType(_Initializer.listType),
		IID_PPV_ARGS(&allocator)));

	m_CmdAllocator = move(unique_ptr<ID3D12CommandAllocator>(allocator));
	return true;
}


bool CCommandContext::CreateCommandList(const FCommandContextInitializer& _Initializer)
{
	ID3D12GraphicsCommandList* list = nullptr;
	D3D_ERROR_CHECK(_Initializer.Device->CreateCommandList(
		_Initializer.NodeMask,
		GetD3DCommandListType(_Initializer.listType),
		m_CmdAllocator.get(),
		_Initializer.PipelineState,
		IID_PPV_ARGS(&list)));

	m_CmdList = move(unique_ptr<ID3D12GraphicsCommandList>(list));
	return true;
}
