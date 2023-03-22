#pragma once

BIFROST_NAMESPACE_BEGIN

class CVmdParser
{
public:

	bool ParseData(const uint8* _Data, uint32 _Size);

	void Reset();


	CVmdParser();
	~CVmdParser();

private:

	class Impl;
	ObjectPtr(Impl) m_Impl = nullptr;

};

BIFROST_NAMESPACE_END