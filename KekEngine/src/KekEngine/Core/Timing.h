#pragma once
#include "Clock.h"
#include "Log.h"

namespace Kek
{
	class Timing
	{
		public:
		static void RestartTiming();
		static float GetTiming();
	};
}

