/////////////////////////////////////////////////////////////////////////////
///@file	main.cpp
///@brief	Entry Point
///@data	2020/06/25
////////////////////////////////////////////////////////////////////////////

//Include Header
#include <eden/include/system/directory/directory_manager.h>
#include <eden/include/math/random_utility.h>
#include <eden/include/math/math_utility.h>

using namespace EDENS_NAMESPACE;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmd)
{
	CDirectoryManager& directoryManager = CDirectoryManager::GetInstance();

	directoryManager.Init();

	CRandomizer& randomUtility = CRandomizer::GetInstance();

	uquad m = randomUtility.RandomRangeInteger<uquad>(0, 10);

	uquad n = CMathUtility::Clamp(3, 0, 1);

	return 0;
}