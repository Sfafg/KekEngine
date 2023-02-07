#include "Log.h"

namespace Kek
{
	IConsole &Debug::Console()
	{
		static WindowsConsole wConsole;
		static IConsole *console = &wConsole;

		return *console;
	}

	template <>
	void Log<LOGTYPE::RAW>(const std::string_view &str)
	{
		Debug::Console() << str << '\n';
	}
	template <>
	void Log<LOGTYPE::NONE>(const std::string_view &str)
	{
		Debug::Console().SetColor("LYellow");
		Debug::Console() << str << '\n';
	}
	template <>
	void Log<LOGTYPE::INFO>(const std::string_view &str)
	{
		auto col = Debug::Console().GetColor();
		Debug::Console().SetColor("LGreen");
		Debug::Console() << "INFO: ";
		Debug::Console().SetColor("LYellow");
		Debug::Console() << str << '\n';
		Debug::Console().SetColor(col);
	}
	template <>
	void Log<LOGTYPE::WARNING>(const std::string_view &str)
	{
		auto col = Debug::Console().GetColor();
		Debug::Console().SetColor("LYellow");
		Debug::Console() << "WARNING: ";
		Debug::Console().SetColor("LYellow");
		Debug::Console() << str << '\n';
		Debug::Console().SetColor(col);
	}
	template <>
	void Log<LOGTYPE::ERROR>(const std::string_view &str)
	{
		auto col = Debug::Console().GetColor();
		Debug::Console().SetColor("Red");
		Debug::Console() << "ERROR: ";
		Debug::Console().SetColor("LYellow");
		Debug::Console() << str << '\n';
		Debug::Console().SetColor(col);
	}
}