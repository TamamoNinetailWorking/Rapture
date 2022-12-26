#include "IndexBuffer.h"

#include <d3d12.h>
#include <Atlantis/DirectX12/DirectXHelper/d3dx12.h>
#include <Atlantis/DirectX12/DirectX12BaseDefine.h>
#include <eden/include/utility/ender_utility.h>

USING_ATLANTIS;

bool CIndexBuffer::Initialize(const FInitializer& _Initializer)
{
    do
    {
        D3D_INIT_PROCESS_CHECK(CreateIndexBuffer(_Initializer));

        return true;
    } while (0);

    Finalize();
    return false;
}

void CIndexBuffer::Finalize()
{
    EDENS_NAMESPACE::Delete(m_View);
    SafeReleaseD3DPtr(m_Resource);
}

bool CIndexBuffer::CreateIndexBuffer(const FInitializer& _Initializer)
{
    D3D_CHECK_NULL(_Initializer.Device);
    D3D_CHECK_NULL(_Initializer.Indices);

    D3D12_HEAP_PROPERTIES heap = {};
    heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

    uint32 indicesSize = _Initializer.IndicesSize;
    D3D12_RESOURCE_DESC desc = {};
    desc = CD3DX12_RESOURCE_DESC::Buffer(indicesSize);

    D3D_ERROR_CHECK(_Initializer.Device->CreateCommittedResource(
        &heap,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_Resource)
    ));

    uint16* map = nullptr;
    if (SUCCEEDED(m_Resource->Map(0, nullptr, (void**)(&map))))
    {
        memcpy_s(map, indicesSize, _Initializer.Indices, indicesSize);
        m_Resource->Unmap(0, nullptr);
    }

    m_View = new D3D12_INDEX_BUFFER_VIEW();
    D3D_CHECK_NULL(m_View);

    m_View->BufferLocation = m_Resource->GetGPUVirtualAddress();
    m_View->Format = DXGI_FORMAT_R16_UINT;
    m_View->SizeInBytes = indicesSize;

    return true;
}
