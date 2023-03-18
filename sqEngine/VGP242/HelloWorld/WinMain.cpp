#include <Core/Inc/Core.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//sqEngine::Core::TimeUtil::GetTime();
	LOG("IM %s.Is %d","sq",42);
	LOG("%f seconds have passed", 1.5f);

	const int n = 42;
	//ASSERT(n == 7, "42 is not 7.");

	return 0;
}

