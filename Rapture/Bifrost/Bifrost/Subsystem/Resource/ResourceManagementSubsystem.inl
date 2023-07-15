#pragma once


BIFROST_NAMESPACE_BEGIN

template <class TManager>
bool CResourceManagementSubsystem::SetupManager(uint32 _Type)
{
	if (_Type >= SCast<uint32>(m_Managers.size())) { return false; }

	m_Managers[_Type] = new TManager();
	return m_Managers[_Type] != nullptr;
}


BIFROST_NAMESPACE_END