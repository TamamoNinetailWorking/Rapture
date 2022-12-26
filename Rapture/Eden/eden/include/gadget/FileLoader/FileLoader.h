#pragma once

#include <vector>

EDENS_NAMESPACE_BEGIN

class CFileLoader
{
public:

	bool FileLoad(const char* _FileName);
	bool FileLoad(Hash160 _FileNameHash);

	void ResetData();

	const Hash160& GetFileName() const { return m_FileName; };
	const uint8* GetData() const { return m_Data.data(); };
	uint32 GetSize() const { return m_DataSize; };

	CFileLoader() {};
	~CFileLoader() {};

private:

	Hash160 m_FileName = {};

	//ObjectPtr(uint8) m_Data = nullptr;
	std::vector<uint8> m_Data = {};
	uint32 m_DataSize = 0;

};



EDENS_NAMESPACE_END