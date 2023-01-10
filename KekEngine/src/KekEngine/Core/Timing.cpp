#include "Timing.h"
#include "Clock.h"

namespace Kek
{
	Clock timingClock;
	void Timing::RestartTiming()
	{
		timingClock.Restart();
	}
	float Timing::GetTiming()
	{
		return timingClock;
	}
}