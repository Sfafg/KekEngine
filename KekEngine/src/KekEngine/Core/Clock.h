#pragma once
#include <iosfwd>
#include <chrono>

namespace Kek
{
	enum TimeFormat
	{
		Nanoseconds = 1000000000,
		Microseconds = 1000000,
		Miliseconds = 1000,
		Seconds = 1
	};
	std::ostream& operator<<(std::ostream& os, const TimeFormat& o);

	template <TimeFormat TFormat = TimeFormat::Seconds>
	struct Clock
	{
		Clock();
		void Restart();
		float operator ()();
		operator float();
		private:
		float GetTime();
		std::chrono::time_point<std::chrono::steady_clock> start;
	};
}