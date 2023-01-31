#include "MaterialInterface.h"
#include "MaterialInterfaceInitializer.h"

#include <d3d12.h>
#include <Atlantis/DirectX12/DirectX12BaseDefine.h>

USING_ATLANTIS;


IMaterialInterface::IMaterialInterface()
{
}

IMaterialInterface::~IMaterialInterface()
{
}

void ATLANTIS_NAMESPACE::IMaterialInterface::SetHeapStride(uint32 _Stride)
{
	m_HeapStride = _Stride;
}

void ATLANTIS_NAMESPACE::IMaterialInterface::Finalize()
{
	SafeReleaseD3DPtr(m_MaterialDescriptorHeap);
}

ID3D12DescriptorHeap* ATLANTIS_NAMESPACE::IMaterialInterface::GetDescriptorHeap() const
{
	return m_MaterialDescriptorHeap;
}

ID3D12DescriptorHeap* const* ATLANTIS_NAMESPACE::IMaterialInterface::GetDescriptorHeapPtr() const
{
	return &m_MaterialDescriptorHeap;
}

ID3D12DescriptorHeap* ATLANTIS_NAMESPACE::IMaterialInterface::GetBufferDescriptorHeap() const
{
	return m_BufferDescriptorHeap;
}

ID3D12DescriptorHeap* const* ATLANTIS_NAMESPACE::IMaterialInterface::GetBufferDescriptorHeapPtr() const
{
	return &m_BufferDescriptorHeap;
}

uint32 ATLANTIS_NAMESPACE::IMaterialInterface::GetHeapStride() const
{
	return m_HeapStride;
}
