#pragma once

#include <memory>

namespace DirectX
{
	struct TexMetadata;
	struct Image;
	class ScratchImage;
};

ATLANTIS_NAMESPACE_BEGIN

class CTexture
{
public:

	struct FInitializer
	{
	public:

		Hash160 FileNameHash = {};
		const void* Data = nullptr;
		uint32 DataSize = 0;

	};

	bool Initialize(const FInitializer& _Initializer);

	void Finalize();


	DirectX::TexMetadata* GetMetaData() const { return m_MetaData; };
	const DirectX::Image* GetImage() const;

	CTexture();
	~CTexture();

private:

	bool CreateTexture(const FInitializer& _Initializer);

	//std::unique_ptr<DirectX::TexMetadata> m_MetaData = nullptr;
	ObjectPtr(DirectX::TexMetadata) m_MetaData = nullptr;
	//std::unique_ptr<DirectX::ScratchImage> m_Image = nullptr;
	ObjectPtr(DirectX::ScratchImage) m_Image = nullptr;

	Hash160 m_FileName = {};
	//Hash160 m_FileExtensionHash = {};

};


ATLANTIS_NAMESPACE_END