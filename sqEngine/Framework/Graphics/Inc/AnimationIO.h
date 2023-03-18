#pragma once
#include "AnimationClip.h"

namespace sqEngine::Graphics
{
	class AnimationIO final
	{
	public:
		static void Write(FILE* file, const Animation& animation);
		static void Read(FILE* file, Animation& animation);
	};
}