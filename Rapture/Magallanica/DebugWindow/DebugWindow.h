#pragma once

#include <string>

MAGALLANICA_NAMESPACE_BEGIN

struct FDebugWindowInitializer;

class CDebugWindow
{
public:

	virtual bool Initialize(const FDebugWindowInitializer* _Initializer);
	virtual void Finalize();

	void ShowWindow(float _DeltaTime);

	bool* OpenWindow();

	const char* GetWindowName() const;

	CDebugWindow() {};
	~CDebugWindow() {};

protected:

	virtual void ShowWindowImpl(float _DeltaTime) {};

protected:

	std::string m_WindowName = {};
	
	bool m_OpenWindow = false;

};

MAGALLANICA_NAMESPACE_END