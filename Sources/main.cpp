#include "pch.h"
#include "Game/Game.h"

extern "C"
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

#ifdef _DEBUG
int main()
#else
int WINAPI wWinMain(HINSTANCE h_instance, HINSTANCE h_prevInstance, wchar_t* cmd, int cmd_show)
#endif
{
	_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::make_unique<Game>()->run();
}