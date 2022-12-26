#include "FileUtility.h"
#include "FileTypeDefine.h"

USING_ATLANTIS;
using namespace std;

bool FileUtility::CompareExtension(const std::string& _extension, EFileExtensionType _type)
{
	if (_type >= FILE_TYPE_NUM) { return false; }
	
	const string ext = FileExtension[_type];
	const string extUp = FileExtensionUpper[_type];

	return _extension == ext || _extension == extUp;
}
