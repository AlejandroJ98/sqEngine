#include<sqEngine/Inc/sqEngine.h>


int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	using namespace sqEngine;
	sqEngine::Core::Window myWindow;
	myWindow.Initialize(instance, L"Hello DirectX", 1280, 720);

	Graphics::GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);

	while (true)
	{
		myWindow.ProcessMessage();
		if (!myWindow.IsActive())
		{
			break;
		}
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			break;
		}

		auto graphicSystem = Graphics::GraphicsSystem::Get();
		if (GetAsyncKeyState('1'))
			graphicSystem->SetClearColor(Graphics::Colors::HotPink);
		if (GetAsyncKeyState('2'))
			graphicSystem->SetClearColor(Graphics::Colors::Black);
		if (GetAsyncKeyState('3'))
			graphicSystem->SetClearColor(Graphics::Colors::Blue);
		if (GetAsyncKeyState('4'))
			graphicSystem->SetClearColor(Graphics::Colors::Green);
		if (GetAsyncKeyState('5'))
			graphicSystem->SetClearColor(Graphics::Colors::Purple);

		graphicSystem->BeginRender();
		//do all the drawing here
		graphicSystem->EndRender();
	}


	Graphics::GraphicsSystem::StaticTerminate();

	myWindow.Terminate();
	return 0;
}