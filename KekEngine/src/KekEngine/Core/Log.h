#pragma once
#include <iosfwd>
#include "Console.h"
#include "KekEngine/Core/Flag.h"

namespace Kek
{
	/// <summary>
	/// Enum that specifies how to Log the string.
	/// </summary>
	enum LogType
	{
		None = 0,
		Raw = 1,
		Info = 2,
		Warning = 4,
		Error = 8
	};

	namespace Debug
	{
		/// <summary>
		/// Console Interface holding refrence to Console object.
		/// </summary>
		IConsole* Console();
#ifndef NDEBUG;
		FlagSet& Mask();
#endif // Debug_Mode;
	};

	/// <summary>
	/// Function Logging string with specific LogType specified as function Template.
	/// </summary>
	/// <param name="str">String_view parameter to log.</param>
	template <LogType Type = LogType::None>
	void Log(const std::string_view& str);


	/// <summary>
	/// Printf-like function using variadic templates.
	/// <para />Formatting format:
	/// <para />[] -> Sequence [start, end]
	/// <para />&lt;&gt; -> Sequence Scope Limits
	/// <para />FG -> Foreground Color
	/// <para />BG -> Background Color
	/// <para /> {ColorName} #FFFFFF (r/g/b) -> Color [0...255]
	/// <para />B -> Bold
	/// <para />_ -> underline
	/// <para />&gt;/&lt;(num) -> add/remove indention
	/// <para />(num) -> set indention
	/// </summary>
	/// <typeparam name="...T"></typeparam>
	/// <param name="...o"></param>
	template <LogType Type = LogType::None, typename... T>
	void Log(const T&... o)
	{
		std::stringstream ss;
		((ss << o), ...);

		Log<Type>((std::string_view)ss.str());
	}
}