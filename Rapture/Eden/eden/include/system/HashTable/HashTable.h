#pragma once

#include <map>
#include <string>

#include <eden/include/system/sha1/sha1.h>
#include <eden/include/template/singleton.h>

EDENS_NAMESPACE_BEGIN

class CHashTable : public CSingleton<CHashTable>
{
public:

	void SetValue(const std::string& _string);
	void SetValue(const char* _string);;

	const Hash160 CreateHash(const std::string& _string);
	const Hash160 CreateHash(const char* _string);

	const Hash160 GetHash(const char* _string) const;
	const Hash160 GetHash(const std::string& _string) const;

	const char* GetHashValue(const Hash160& _hash) const;


private:

	typedef std::map<Hash160,std::string> HashTable;
	
	HashTable m_HashTable = {};
};


EDENS_NAMESPACE_END

#define CHash160(str) EDENS_NAMESPACE::CHashTable::GetInstance().CreateHash(str)
#define RHash160(hash) EDENS_NAMESPACE::CHashTable::GetInstance().GetHashValue(hash)