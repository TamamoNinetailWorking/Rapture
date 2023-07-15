#pragma once

/*
*    TextureやシェーダーのFileLoad周りを分離して、
    Memoryからデータ作成するAPIに差し替えてもいいかも。
    　>>今は特に困らないが、
    　　プラットフォームが変わったときにファイルシステムが差し替えられる。
    GraphicsShaderも
        >> 分離しないといけないのはShaderBase側に記述が多そう
*/

#include <eden/include/Resource/Resource.h>

//struct ID3D12Device;
struct ID3D12Resource;

EDENS_NAMESPACE_BEGIN

struct FResourceInitializerBase;

EDENS_NAMESPACE_END

ATLANTIS_NAMESPACE_BEGIN

struct FTextureInitializer;

class CTextureResource : public EDENS_NAMESPACE::CResource
{
public:

	bool Initialize(const EDENS_NAMESPACE::FResourceInitializerBase* _initializer) override;
	void Finalize() override;

	ID3D12Resource* GetResource() const { return m_Resource; };

private:

	bool CreateTextureResource(const FTextureInitializer* _Initializer);
	bool CopyToVideoMemory(const FTextureInitializer* _Initializer);

	ObjectPtr(ID3D12Resource) m_Resource = nullptr;

};

ATLANTIS_NAMESPACE_END