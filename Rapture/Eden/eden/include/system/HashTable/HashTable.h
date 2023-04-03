#pragma once

#include <map>
#include <vector>
#include <string>

#include <eden/include/system/sha1/sha1.h>
#include <eden/include/template/singleton.h>

// 文字列とデータの塊のハッシュは別で持つかな・・・
// SHA1とGUIDとか、使い分けた方がデータ扱いやすいかもしれない

EDENS_NAMESPACE_BEGIN

class CHashTable : public CSingleton<CHashTable>
{
public:

	using DataBlob = std::vector<uint8>;
	using String = std::string;

	void SetValue(const DataBlob& _Data);
	void SetValue(const String& _string);
	void SetValue(const char* _string);

	const Hash160 CreateHash(const DataBlob& _Data);
	const Hash160 CreateHash(const String& _string);
	const Hash160 CreateHash(const char* _string);
	const Hash160 CreateHash(const uint8* _string);

	const Hash160 GetHash(const DataBlob& _Data) const;
	const Hash160 GetHash(const char* _string) const;
	const Hash160 GetHash(const String& _string) const;
	const Hash160 GetHash(const uint8* _string) const;

	const uint8* GetHashData(const Hash160& _Hash) const;
	const char* GetHashValue(const Hash160& _hash) const;


private:

	typedef std::map<Hash160,String> StringHashTable;
	typedef std::map<Hash160,DataBlob> HashTable;
	
	StringHashTable m_StringHashTable = {};
	HashTable m_HashTable = {};
};


EDENS_NAMESPACE_END

#define CHash160(str) EDENS_NAMESPACE::CHashTable::GetInstance().CreateHash(str)
#define RHash160(hash) EDENS_NAMESPACE::CHashTable::GetInstance().GetHashValue(hash)
#define DHash160(hash) EDENS_NAMESPACE::CHashTable::GetInstance().GetHashData(hash)
static const Hash160 INVALID_HASH = Hash160();