#pragma once


#include <Magallanica/DebugWindow/DebugWindow.h>

class CDebugPostEffect : public MAGALLANICA_NAMESPACE::CDebugWindow
{
public:

	CDebugPostEffect();

protected:

	void ShowWindowImpl(float _DeltaTime) override;

};