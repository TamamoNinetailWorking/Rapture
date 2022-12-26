/////////////////////////////////////////////////////////////////////////////
///@file	directory_manager.h
///@brief	directory_manager	define
///@data	2020/06/25
////////////////////////////////////////////////////////////////////////////
#ifndef __DIRECTORY_MANAGER_H__
#define __DIRECTORY_MANAGER_H__

//Include Header
#include <eden/include/template/singleton.h>

EDENS_NAMESPACE_BEGIN

/*
*	Pointer Directory Path
*/
class CDirectoryManager : public CSingleton<CDirectoryManager>
{
protected:

	friend class CSingleton<CDirectoryManager>;

public:

	~CDirectoryManager() { Exit(); };

	void Init(u32 name_size = 1024);
	void Exit();

	void RestoreDefaultDirectory();
	void SetDirectory(const c8* directory);

protected:

	c8* m_default_current_directory = nullptr;
	u32 m_directory_name_size = 0;

};

EDENS_NAMESPACE_END

#endif