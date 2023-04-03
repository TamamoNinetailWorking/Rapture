#include <eden/include/system/HashTable/HashTable.h>


using namespace std;
EDENS_NAMESPACE_USING;

void CHashTable::SetValue(const DataBlob& _Data)
{
	Hash160 sha1(_Data);
	m_HashTable[sha1] = _Data;
}

void CHashTable::SetValue(const String& _string)
{
	//CHashTable::DataBlob Data(_string.begin(), _string.end());
	//SetValue(Data);
	Hash160 sha1(_string);
	m_StringHashTable[sha1] = _string;
}

void CHashTable::SetValue(const char* _string)
{
	std::string message(_string);
	SetValue(message);
	/*CSHA1 sha1(_string);
	///m_HashTable.insert(make_pair(sha1, _string));
	m_HashTable[sha1] = string(_string);*/
}

const Hash160 CHashTable::CreateHash(const DataBlob& _Data)
{
	Hash160 sha1 = GetHash(_Data);
	if (sha1 != Hash160()) { return sha1; }

	sha1 = Hash160(_Data);
	m_HashTable[sha1] = _Data;
	return sha1;
}

const Hash160 CHashTable::CreateHash(const std::string& _String)
{
	Hash160 sha1 = GetHash(_String);
	if (sha1 != Hash160()) { return sha1; }

	sha1 = Hash160(_String);
	m_StringHashTable[sha1] = _String;
	return sha1;
}

const Hash160 CHashTable::CreateHash(const char* _string)
{
	std::string message(_string);
	return CreateHash(message);
/*	Hash160 sha1 = GetHash(_string);
	if (sha1 != Hash160()) { return sha1; }

	sha1 = Hash160(_string);
	m_HashTable[sha1] = _string;
	return sha1*/;
}

const Hash160 CHashTable::CreateHash(const uint8* _string)
{
	const char* temp = RCast<const char*>(_string);
	return CreateHash(temp);
}

const Hash160 CHashTable::GetHash(const DataBlob& _Data) const
{
	for (auto& elem : m_HashTable)
	{
		if (elem.second == _Data)
		{
			return elem.first;
		}
	}

	return Hash160();
}

const Hash160 CHashTable::GetHash(const std::string& _string) const
{
	for (auto& elem : m_StringHashTable)
	{
		if (elem.second == _string)
		{
			return elem.first;
		}
	}

	return Hash160();
}

const Hash160 CHashTable::GetHash(const char* _string) const
{
	string message(_string);
	return GetHash(message);
}

const Hash160 CHashTable::GetHash(const uint8* _string) const
{
	const char* temp = RCast<const char*>(_string);
	return GetHash(temp);
}

const uint8* CHashTable::GetHashData(const Hash160& _Hash) const
{
	const auto& itr = m_HashTable.find(_Hash);
	if (itr == m_HashTable.cend()) { return nullptr; }
	return itr->second.data();
}

const char* CHashTable::GetHashValue(const Hash160& _hash) const
{
	const auto& itr = m_StringHashTable.find(_hash);
	if (itr == m_StringHashTable.cend()) { return nullptr; }
	return itr->second.c_str();
}
