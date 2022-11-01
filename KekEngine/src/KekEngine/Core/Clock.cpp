#include "Clock.h"
#include "KekEngine/Core/Log.h"

namespace Kek
{
	std::ostream& operator<<(std::ostream& os, const TimeFormat& o)
	{
		switch(o)
		{
		case TimeFormat::Seconds: os << "sec"; return os;
		case TimeFormat::Miliseconds: os << "ms"; return os;
		case TimeFormat::Microseconds: os << "mics"; return os;
		case TimeFormat::Nanoseconds: os << "ns"; return os;
		default:return os;
		};
		return os;
	}

	template<TimeFormat TFormat> Clock<TFormat>::Clock()
	{
		Restart();
	}
	template<TimeFormat TFormat> void Clock<TFormat>::Restart()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	template<TimeFormat TFormat> float Clock<TFormat>::operator ()() { return GetTime(); }
	template<TimeFormat TFormat> Clock<TFormat>::operator float() { return GetTime(); }
	template<TimeFormat TFormat> float Clock<TFormat>::GetTime()
	{
		std::chrono::duration<float> dur = std::chrono::high_resolution_clock::now() - start;
		return dur.count() * TFormat;
	}


	template class Clock<Nanoseconds>;
	template class Clock<Microseconds>;
	template class Clock<Miliseconds>;
	template class Clock<Seconds>;
}