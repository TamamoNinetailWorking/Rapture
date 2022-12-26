#include "FileLoader.h"

//#include <stdio.h>
#include <fstream>
#include "eden/include/utility/StringUtility.h"

EDENS_NAMESPACE_USING;

using namespace std;

bool CFileLoader::FileLoad(const char* _FileName)
{
    errno_t result = {};

    ifstream fp = {};
    fp.open(_FileName, ios_base::binary);

    if (!fp) { return false; }

    fp.seekg(0, ios_base::end);

    m_DataSize = SCast<uint32>(fp.tellg());
    //PRINT("CFileLoader::FileLoad::FileSize %u\n", m_DataSize);

    m_Data.resize(m_DataSize);

    fp.seekg(0, ios_base::beg);

    fp.read(RCast<sint8*>(m_Data.data()), m_DataSize);

    fp.close();

    m_FileName = CHash160(_FileName);

    return true;
}

bool CFileLoader::FileLoad(Hash160 _FileNameHash)
{
    return FileLoad(RHash160(_FileNameHash));
}

void CFileLoader::ResetData()
{
    if (m_Data.data())
    {
        m_Data.resize(0);
    }
}
