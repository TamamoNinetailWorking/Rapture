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

	void Init(uquad name_size = 1024);
	void Exit();

	void RestoreDefaultDirectory();
	void SetDirectory(const unichar* directory);

protected:

	unichar* m_default_current_directory = nullptr;
	uquad m_directory_name_size = 0;

};

EDENS_NAMESPACE_END

#endif