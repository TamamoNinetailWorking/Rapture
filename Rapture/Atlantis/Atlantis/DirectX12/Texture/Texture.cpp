#include "Texture.h"

#include <string>

#include <eden/include/utility/ender_utility.h>

#include <Atlantis/DirectX12/DirectXTex/DirectXTex.h>
#include <Atlantis/DirectX12/DirectX12BaseDefine.h>
#include <Atlantis/DirectX12/Utility/DirectXUtility.h>

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
	ResetPtr(m_Image);
	ResetPtr(m_MetaData);
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

	const uint32 fileNameLength = 512;
	WCHAR fileName[fileNameLength] = {};
	{
		size_t length = 0;
		int32 error = Utility::ToWString(fileName, fileNameLength, RHash160(_Initializer->FileNameHash), length);
		if (error == EINVAL) { return false; }
	}

	wstring path(fileName);

	size_t index = path.find_last_of(L'.');
	wstring extension = path.substr(index + 1, path.length() - index);
	  

	{
		HRESULT result = S_OK;

		if ((extension == L"tga") || (extension == L"TGA"))
		{
			result = LoadFromTGAFile(fileName, metaData, *scratchImage);
		}
		else if ((extension == L"dds") || (extension == L"DDS"))
		{
				result = LoadFromDDSFile(fileName, DDS_FLAGS_NONE, metaData, *scratchImage);
		}
		else
		{
			result = LoadFromWICFile(fileName, WIC_FLAGS_NONE, metaData, *scratchImage);
		}

		D3D_ERROR_CHECK(result);
	}

	m_MetaData.reset(metaData);
	m_Image.reset(scratchImage);

	return true;
}
