#include <eden/include/system/HashTable/HashTable.h>


using namespace std;
EDENS_NAMESPACE_USING;

void CHashTable::SetValue(const std::string& _string)
{
	Hash160 sha1(_string);
	m_HashTable[sha1] = _string;
}

void CHashTable::SetValue(const char* _string)
{
	CSHA1 sha1(_string);
	///m_HashTable.insert(make_pair(sha1, _string));
	m_HashTable[sha1] = string(_string);
}

const Hash160 CHashTable::CreateHash(const std::string& _string)
{
	return CreateHash(_string.c_str());
}

const Hash160 CHashTable::CreateHash(const char* _string)
{
	Hash160 sha1 = GetHash(_string);
	if (sha1 != Hash160()) { return sha1; }

	sha1 = Hash160(_string);
	m_HashTable[sha1] = _string;
	return sha1;
}

const Hash160 CHashTable::GetHash(const char* _string) const
{
	for (auto& elem : m_HashTable)
	{
		if (elem.second == string(_string))
		{
			return elem.first;
		}
	}

	return Hash160();
}

const Hash160 CHashTable::GetHash(const std::string& _string) const
{
	return GetHash(_string.c_str());
}

const char* CHashTable::GetHashValue(const Hash160& _hash) const
{
	const auto& itr = m_HashTable.find(_hash);
	if (itr == m_HashTable.cend()) { return nullptr; }
	return itr->second.c_str();
}
