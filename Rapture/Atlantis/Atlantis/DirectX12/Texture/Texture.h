#pragma once

#include <memory>
#include <eden/include/Resource/Resource.h>

namespace DirectX
{
	struct TexMetadata;
	struct Image;
	class ScratchImage;
};

ATLANTIS_NAMESPACE_BEGIN

class CTexture : public EDENS_NAMESPACE::CResource
{
public:

	struct FInitializer : public CResource::FInitializerBase
	{
	public:

		Hash160 FileNameHash = {};

	};

	bool Initialize(const FInitializerBase* _Initializer) override;

	void Finalize() override;


	DirectX::TexMetadata* GetMetaData() const { return m_MetaData.get(); };
	const DirectX::Image* GetImage() const;

	CTexture() {};
	CTexture(const CTexture& _resource);
	~CTexture();

private:

	bool CreateTexture(const FInitializer* _Initializer);

	std::unique_ptr<DirectX::TexMetadata> m_MetaData = nullptr;
	std::unique_ptr<DirectX::ScratchImage> m_Image = nullptr;

};


ATLANTIS_NAMESPACE_END