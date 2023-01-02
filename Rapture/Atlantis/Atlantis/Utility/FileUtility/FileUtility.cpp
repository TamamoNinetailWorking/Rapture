#include "FileUtility.h"
#include "FileTypeDefine.h"

#include <eden/include/utility/StringUtility.h>

USING_ATLANTIS;
using namespace std;

bool FileUtility::CompareExtension(const std::string& _extension, EFileExtensionType _type)
{
	if (_type >= FILE_TYPE_NUM) { return false; }
	
	const string ext = FileExtension[_type];
	const string extUp = FileExtensionUpper[_type];

	return _extension == ext || _extension == extUp;
}

const std::string FileUtility::ConvertTexturePathWithModel(const std::string& _ModelPath, const std::string& _TexturePath)
{
	return ConvertTexturePathWithModel(_ModelPath, _TexturePath.c_str());
}

const std::string FileUtility::ConvertTexturePathWithModel(const std::string& _ModelPath, const char* _TexturePath)
{
	if (strlen(_TexturePath) == 0) { return string(); }
	int32 pathIndex1 = SCast<uint32>(_ModelPath.rfind('\\'));
	int32 pathIndex2 = SCast<uint32>(_ModelPath.rfind('/'));
	int32 index = max(pathIndex1, pathIndex2);

	string folderPath = _ModelPath.substr(0, index + 1);

	return folderPath + _TexturePath;
}

const std::string ATLANTIS_NAMESPACE::FileUtility::GetExtension(const std::string& _Path)
{
	int32 index = SCast<int32>(_Path.rfind('.'));

	return _Path.substr(index + 1, _Path.length() - index - 1);
}

bool ATLANTIS_NAMESPACE::FileUtility::CompareExtensionFromPath(const String& _Path, EFileExtensionType _Type)
{
	const string extension = GetExtension(_Path);
	return CompareExtension(extension, _Type);
}

std::pair<std::string, std::string> ATLANTIS_NAMESPACE::FileUtility::SeparateFileName(const String& _Path, const char _Splitter)
{
	return EDENS_NAMESPACE::StringUtility::SeparateText(_Path, _Splitter);
}
