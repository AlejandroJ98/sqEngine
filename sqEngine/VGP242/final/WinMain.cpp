#include "GameState.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	auto& app = sqEngine::MainApp();
	app.AddState<GameState>("GameState");
	app.Run({ L"final", 1280, 720 });
	return 0;
}