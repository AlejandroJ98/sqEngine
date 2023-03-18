#include "GameState.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	sqEngine::AppConfig appConfig;
	std::wstring appName = L"sqEngine";
	uint32_t winWidth = 1280;
	uint32_t winHeight = 720;
	uint32_t debugDrawLimit = 100000;
	std::filesystem::path textureRoot = "../../Assets/Images";
	
	auto& app = sqEngine::MainApp();
	app.AddState<GameState>("GameState");
	app.Run(appConfig);
	return 0;
}

