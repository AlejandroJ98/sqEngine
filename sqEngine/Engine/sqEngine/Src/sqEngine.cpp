#include "Precompiled.h"
#include "sqEngine.h"

sqEngine::App& sqEngine::MainApp()
{
	static App sApp;
	return sApp;
}

