#pragma once

struct ID3D12DescriptorHeap;

ATLANTIS_NAMESPACE_BEGIN

struct FMaterialInterfaceInitializerBase;

class CGraphicsPipeline;
class CRootSignature;

struct FMaterialGeometryBufferBase;

class IMaterialInterface
{
public:

	virtual bool Initialize(const FMaterialInterfaceInitializerBase* _Initializer) = 0;
	virtual void Finalize();

	ID3D12DescriptorHeap* GetDescriptorHeap() const;
	ID3D12DescriptorHeap* const* GetDescriptorHeapPtr() const;
	uint32 GetHeapStride() const;
	virtual uint32 GetDrawIndex(uint32 _Index) const = 0;
	virtual uint32 GetMaterialNum() const = 0;

	virtual CGraphicsPipeline* GetGraphicsPipeline() const = 0;
	virtual CRootSignature* GetRootSignature() const = 0;

	virtual const FMaterialGeometryBufferBase* GetGeometryBuffer() const = 0;
	virtual FMaterialGeometryBufferBase* GetGeometryBufferEdit() const = 0;

	virtual void SetGeometryBuffer(FMaterialGeometryBufferBase* _Buffer) = 0;

	// �����͂܂Ƃ߂邩�A�ʂ̃N���X�ɂ�����
	// �`��e�X�g�ł͈ꉞ�ʂ����̂ŁA�܂Ƃ߂Ė��Ȃ��Ǝv����
	// ���ǖ��ɂȂ�̂�GPU�������Ԃ�CPU�������ԂƂ̌��ˍ����Ȃ̂ŁA�����̏������Ԃ𑪂肽��
	ID3D12DescriptorHeap* GetBufferDescriptorHeap() const;
	ID3D12DescriptorHeap* const* GetBufferDescriptorHeapPtr() const;

	IMaterialInterface();
	~IMaterialInterface();

protected:

	ObjectPtr(ID3D12DescriptorHeap) m_BufferDescriptorHeap = nullptr;
	ObjectPtr(ID3D12DescriptorHeap) m_MaterialDescriptorHeap = nullptr;

	void SetHeapStride(uint32 _Stride);

private:

	uint32 m_HeapStride = 0;

};

ATLANTIS_NAMESPACE_END