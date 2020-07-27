/////////////////////////////////////////////////////////////////////////////
///@file	directory_manager.cpp
///@brief	directory_manager	Implementation
///@data	2020/06/25
////////////////////////////////////////////////////////////////////////////

//Include Header
#include <eden/include/system/directory/directory_manager.h>

EDENS_NAMESPACE_BEGIN

//CDirectoryManager::~CDirectoryManager()
//{
//	Exit();
//}

void CDirectoryManager::Init(u32 name_size)
{
	m_directory_name_size = name_size;
	m_default_current_directory = new c8[m_directory_name_size];

	GetCurrentDirectory(m_directory_name_size, m_default_current_directory);
}

void CDirectoryManager::Exit()
{
	if (m_default_current_directory != nullptr)
	{
		delete[] m_default_current_directory;
		m_default_current_directory = nullptr;
	}
}

void CDirectoryManager::RestoreDefaultDirectory()
{
	SetCurrentDirectory(m_default_current_directory);
}

void CDirectoryManager::SetDirectory(const c8* directory)
{
	SetCurrentDirectory(directory);
}

EDENS_NAMESPACE_END