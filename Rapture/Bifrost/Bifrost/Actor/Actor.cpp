#include <Bifrost/Actor/Actor.h>

#include <eden/include/utility/ender_utility.h>

#include <Bifrost/Component/Component.h>

USING_BIFROST;

CActor::CActor()
{
	m_ComponentList = new ComponentList();
	m_ComponentList->reserve(10); // ‚Æ‚è‚ ‚¦‚¸‚ÅƒTƒCƒY’¼‘‚«
}

CActor::~CActor()
{
	EDENS_NAMESPACE::Delete(m_ComponentList);
}

void CActor::Finalize()
{
	for (auto& elem : *m_ComponentList)
	{
		elem->Finalize();
		delete elem;
	}

	m_ComponentList->clear();
}

const Hash160& CActor::GetHash() const
{
	return m_ActorHash;
}

bool CActor::AttachComponent(CComponent* _Component)
{
	CHECK_RESULT_FALSE(_Component);
	CHECK_RESULT_FALSE(m_ComponentList);

	m_ComponentList->push_back(_Component);
	return true;
}

bool CActor::DetachComponent(CComponent* _Component)
{
	CHECK_RESULT_FALSE(_Component);
	CHECK_RESULT_FALSE(m_ComponentList);

	bool result = false;

	for (auto itr = m_ComponentList->begin(); itr != m_ComponentList->end(); ++itr)
	{
		if (*itr != _Component)
		{
			continue;
		}

		itr = m_ComponentList->erase(itr);
		return result;
	}
	
	return result;
}

bool CActor::IsActive() const
{
	return m_IsActive;
}

void CActor::SetActive(bool _IsActive)
{
	m_IsActive = _IsActive;
}

bool CActor::CanEverUpdate() const
{
	return m_CanEverUpdate;
}

void CActor::SetActorHash(const Hash160& _Hash)
{
	m_ActorHash = _Hash;
}

const CActor::ComponentList* CActor::GetComponentList() const
{
	return m_ComponentList;
}
