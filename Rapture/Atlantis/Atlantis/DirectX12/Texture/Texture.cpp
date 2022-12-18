#include "Texture.h"

#include <string>

#include <eden/include/utility/ender_utility.h>

#include <Atlantis/DirectX12/DirectXTex/DirectXTex.h>
#include <Atlantis/DirectX12/DirectX12BaseDefine.h>
#include <eden/include/utility/StringUtility.h>

USING_ATLANTIS;

EDENS_NAMESPACE_USING;

using namespace std;
using namespace DirectX;

bool CTexture::Initialize(const FInitializerBase* _Initializer)
{
	do
	{
		CHECK_RESULT_BREAK(_Initializer);
		
		const FInitializer* init = PCast<const FInitializer*>(_Initializer);
		m_ResourceName = init->FileNameHash;
		
		CHECK_RESULT_BREAK(CreateTexture(init));

		return true;
	} while (0);

	Finalize();
	return false;
}

void CTexture::Finalize()
{
	//ResetPtr(m_Image);
	if (m_Image)
	{
		delete m_Image;
		m_Image = nullptr;
	}
	//ResetPtr(m_MetaData);
	if (m_MetaData)
	{
		delete m_MetaData;
		m_MetaData = nullptr;
	}
}

const Image* CTexture::GetImage() const
{
	return m_Image->GetImage(0,0,0);
}

CTexture::~CTexture()
{
	CResource::~CResource();
}

bool CTexture::CreateTexture(const FInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(!(_Initializer->FileNameHash == Hash160()));

	TexMetadata* metaData = new TexMetadata();
	CHECK_RESULT_FALSE(metaData);
	ScratchImage* scratchImage = new ScratchImage();
	CHECK_RESULT_FALSE(scratchImage);

	const string& fileHash = RHash160(_Initializer->FileNameHash);

	const uint32 fileNameLength = 512;
	WCHAR fileName[fileNameLength] = {};
	{
		size_t length = 0;
		int32 error = StringUtility::ToWString(fileName, fileNameLength, fileHash.c_str(), length);
		if (error == EINVAL) { return false; }
	}

	wstring path(fileName);

	size_t index = path.find_last_of(L'.');
	//wstring extension = path.substr(index + 1, path.length() - index);
	string extension = fileHash.substr(index + 1, fileHash.length() - index);

	{
		HRESULT result = S_OK;

		if ((extension == "tga") || (extension == "TGA"))
		{
			result = LoadFromTGAFile(fileName, metaData, *scratchImage);
		}
		else if ((extension == "dds") || (extension == "DDS"))
		{
				result = LoadFromDDSFile(fileName, DDS_FLAGS_NONE, metaData, *scratchImage);
		}
		else
		{
			result = LoadFromWICFile(fileName, WIC_FLAGS_NONE, metaData, *scratchImage);
		}

		D3D_ERROR_CHECK(result);
	}

	//m_MetaData.reset(metaData);
	m_MetaData = metaData;
	//m_Image.reset(scratchImage);
	m_Image = scratchImage;

	m_FileExtensionHash = CHash160(extension);

	return true;
}
