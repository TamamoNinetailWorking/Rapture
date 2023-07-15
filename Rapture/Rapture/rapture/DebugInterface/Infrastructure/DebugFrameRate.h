#pragma once

#include <Magallanica/DebugWindow/DebugWindow.h>

class CDebugFrameRate : public MAGALLANICA_NAMESPACE::CDebugWindow
{
public:

	CDebugFrameRate();

protected:

	void ShowWindowImpl(float _DeltaTime) override;

};