#include "Texture.h"

#include <string>

#include <eden/include/utility/ender_utility.h>

#include <Atlantis/DirectX12/DirectXTex/DirectXTex.h>
#include <Atlantis/DirectX12/DirectX12BaseDefine.h>
#include <eden/include/utility/StringUtility.h>
#include <Atlantis/Utility/FileUtility/FileUtility.h>

USING_ATLANTIS;

EDENS_NAMESPACE_USING;

using namespace std;
using namespace DirectX;

bool CTexture::Initialize(const FInitializer& _Initializer)
{
	do
	{		
		CHECK_RESULT_BREAK(CreateTexture(_Initializer));

		return true;
	} while (0);

	Finalize();
	return false;
}

void CTexture::Finalize()
{
	Delete(m_Image);
	Delete(m_MetaData);
}

const Image* CTexture::GetImage() const
{
	return m_Image->GetImage(0,0,0);
}

bool CTexture::CreateTexture(const FInitializer& _Initializer)
{
	CHECK_RESULT_FALSE(!(_Initializer.FileNameHash == Hash160()));
	CHECK_RESULT_FALSE(_Initializer.Data);

	TexMetadata* metaData = new TexMetadata();
	CHECK_RESULT_FALSE(metaData);
	ScratchImage* scratchImage = new ScratchImage();
	CHECK_RESULT_FALSE(scratchImage);

	const string& fileHash = RHash160(_Initializer.FileNameHash);

	constexpr uint32 fileNameLength = 512;
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

		//if ((extension == "tga") || (extension == "TGA"))
		if(FileUtility::CompareExtension(extension,FileUtility::EFileExtensionType::FILE_TYPE_TGA))
		{
			//result = LoadFromTGAFile(fileName, metaData, *scratchImage);
			result = LoadFromTGAMemory(_Initializer.Data,_Initializer.DataSize, metaData, *scratchImage);
		}
		//else if ((extension == "dds") || (extension == "DDS"))
		else if (FileUtility::CompareExtension(extension,FileUtility::EFileExtensionType::FILE_TYPE_DDS))
		{
			//result = LoadFromDDSFile(fileName, DDS_FLAGS_NONE, metaData, *scratchImage);
			result = LoadFromDDSMemory(_Initializer.Data,_Initializer.DataSize, DDS_FLAGS_NONE, metaData, *scratchImage);
		}
		else
		{
#if 0
			if (_Initializer.Data == nullptr)
			{
				result = LoadFromWICFile(fileName, WIC_FLAGS_NONE, metaData, *scratchImage);
			}
			else
			{
				result = LoadFromWICMemory(_Initializer.Data, _Initializer.DataSize, WIC_FLAGS_NONE, metaData, *scratchImage);
				PRINT("WIC Memory Load\n");
				if (SUCCEEDED(result))
				{
					PRINT("On Memory load Success.\n");
				}
			}
#endif
			result = LoadFromWICMemory(_Initializer.Data, _Initializer.DataSize, WIC_FLAGS_NONE, metaData, *scratchImage);
		}

		D3D_ERROR_CHECK(result);
	}

	//m_MetaData.reset(metaData);
	m_MetaData = metaData;
	//m_Image.reset(scratchImage);
	m_Image = scratchImage;

	m_FileName = _Initializer.FileNameHash;
	//m_FileExtensionHash = CHash160(extension);

	return true;
}
