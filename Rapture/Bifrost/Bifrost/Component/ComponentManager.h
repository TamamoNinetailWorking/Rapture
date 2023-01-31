#pragma once

#include <list>

BIFROST_NAMESPACE_BEGIN

class CComponent;

class CComponentManager
{
public:

	bool Initialize();
	void Finalize();

	void Update(float _DeltaTime);

	CComponentManager();
	~CComponentManager();

private:

	using ComponentList = std::list<CComponent*>;

	ObjectPtr(ComponentList) m_ComponentList = nullptr;

	void ListFinalize();

};

BIFROST_NAMESPACE_END