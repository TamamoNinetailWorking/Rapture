#pragma once


ATLANTIS_NAMESPACE_BEGIN

namespace FileUtility
{
	enum EFileExtensionType : uint32
	{
		FILE_TYPE_TGA = 0,
		FILE_TYPE_DDS,
		FILE_TYPE_SPH,
		FILE_TYPE_SPA,


		FILE_TYPE_NUM,
	};

	bool CompareExtension(const std::string& _extension, EFileExtensionType _type);
};


ATLANTIS_NAMESPACE_END