#pragma once
#include <iosfwd>
#include <source_location>
#include "Console.h"

namespace Kek
{
	enum LOGTYPE
	{
		NONE = 0,
		RAW = 1,
		INFO = 2,
		WARNING = 4,
		ERROR = 8
	};

	namespace Debug
	{
		IConsole &Console();
	};

	template <LOGTYPE Type = LOGTYPE::NONE>
	void Log(const std::string_view &str);

	template <LOGTYPE Type = LOGTYPE::NONE, typename... T>
	void Log(const T &...o)
	{
		std::stringstream ss;
		((ss << o << ' '), ...);

		Log<Type>((std::string_view)ss.str());
	}
}