#pragma once


ATLANTIS_NAMESPACE_BEGIN

namespace FileUtility
{
	enum class EFileExtensionType : uint32
	{
		FILE_TYPE_TGA = 0,
		FILE_TYPE_DDS,
		FILE_TYPE_SPH,
		FILE_TYPE_SPA,

		FILE_TYPE_HLSL,
		FILE_TYPE_CSO,

		FILE_TYPE_NUM,
	};

	using String = std::string;

	const String GetExtension(const String& _Path);

	bool CompareExtension(const String& _extension, EFileExtensionType _type);

	bool CompareExtensionFromPath(const String& _Path, EFileExtensionType _Type);

	const String ConvertTexturePathWithModel(const String& _Modelpath, const char* _TexturePath);
	const String ConvertTexturePathWithModel(const String& _Modelpath, const String& _TexturePath);

	using FileName01 = std::string;
	using FileName02 = std::string;
	std::pair<FileName01, FileName02> SeparateFileName(const String& _Path, const char _Splitter);
};


ATLANTIS_NAMESPACE_END