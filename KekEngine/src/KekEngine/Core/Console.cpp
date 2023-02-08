#include <Windows.h>
#include <map>
#include "Console.h"
#include "KekEngine/Maths/Color.h"
#include "KekEngine/Maths/Byte.h"

namespace Kek
{
	ColorTable IConsole::colorTable;
	rgb8i &TableColor(int index)
	{
		static rgb8i colorTable[]{
			rgb8i(242, 242, 242),
			rgb8i(12, 12, 12),
			rgb8i(0, 55, 218),
			rgb8i(19, 161, 14),
			rgb8i(58, 150, 221),
			rgb8i(197, 15, 31),
			rgb8i(136, 23, 152),
			rgb8i(193, 156, 0),
			rgb8i(118, 118, 118),
			rgb8i(192, 192, 192),
			rgb8i(59, 120, 255),
			rgb8i(22, 198, 12),
			rgb8i(97, 214, 214),
			rgb8i(231, 72, 86),
			rgb8i(180, 0, 158),
			rgb8i(249, 241, 165)};

		return colorTable[index];
	}
	std::map<std::string, int> &ColorMap()
	{
		static std::map<std::string, int> colorMap{
			{"White", 0},
			{"Black", 1},
			{"Blue", 2},
			{"Green", 3},
			{"Cyan", 4},
			{"Red", 5},
			{"Magenta", 6},
			{"Yellow", 7},
			{"Gray", 8},
			{"LGray", 9},
			{"LBlue", 10},
			{"LGreen", 11},
			{"LCyan", 12},
			{"LRed", 13},
			{"LMagenta", 14},
			{"LYellow", 15}};

		return colorMap;
	}

	bool ColorTable::isValidName(const std::string &str)
	{
		return ColorMap().contains(str);
	}
	rgb8i ColorTable::operator[](const std::string &str)
	{
		auto search = ColorMap().find(str);
		if (search == ColorMap().end())
		{
			return rgb8i(255, 255, 255);
		}
		return TableColor(search->second);
	}
	rgb8i ColorTable::operator[](int ind)
	{
		return TableColor(ind);
	}

	void IConsole::SetColor(const std::string &colorName) { SetColor(colorTable[colorName]); }
	void IConsole::SetBackColor(const std::string &colorName) { SetBackColor(colorTable[colorName]); }

#pragma region WindowsConsole
	HANDLE &HConsole()
	{
		static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		DWORD dwMode = 0;
		GetConsoleMode(hConsole, &dwMode);
		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(hConsole, dwMode);

		return hConsole;
	}
	Byte &WindowsColorTable(int index)
	{
		static Byte windowsColorTable[]{
			Byte(0b00001111),
			Byte(0b00000000),
			Byte(0b00000001),
			Byte(0b00000010),
			Byte(0b00000011),
			Byte(0b00000100),
			Byte(0b00000101),
			Byte(0b00000110),
			Byte(0b00001000),
			Byte(0b00000111),
			Byte(0b00001001),
			Byte(0b00001010),
			Byte(0b00001011),
			Byte(0b00001100),
			Byte(0b00001101),
			Byte(0b00001110),
		};

		return windowsColorTable[index];
	}
	void SetWindowsConsoleColor(const rgb8i &font, const rgb8i &background)
	{
		int fontIndex = 0;
		int backIndex = 0;

		float fontMinV = Color::Similarity(IConsole::colorTable[0], font);
		float backMinV = Color::Similarity(IConsole::colorTable[0], background);
		for (int i = 1; i < 16; i++)
		{
			float fontV = Color::Similarity(IConsole::colorTable[i], font);
			float backV = Color::Similarity(IConsole::colorTable[i], background);
			if (fontV < fontMinV)
			{
				fontMinV = fontV;
				fontIndex = i;
			}
			if (backV < backMinV)
			{
				backMinV = backV;
				backIndex = i;
			}
		}
		Byte fontColorByte = WindowsColorTable(fontIndex);
		Byte backColorByte = WindowsColorTable(backIndex) << 4;
		SetConsoleTextAttribute(HConsole(), backColorByte | fontColorByte);
	}

	void WindowsConsole::ShowCursor(bool show)
	{
		CONSOLE_CURSOR_INFO cursorInfo;

		GetConsoleCursorInfo(HConsole, &cursorInfo);
		cursorInfo.bVisible = show;
		SetConsoleCursorInfo(HConsole, &cursorInfo);
	}
	void WindowsConsole::SetCursorPosition(vec2i pos)
	{
		SetConsoleCursorPosition(HConsole(), COORD{(short)pos.x, (short)pos.y});
	}

	void WindowsConsole::SetIndention(int indention)
	{
		indentionLevel = Maths::Clamp(indention, 0, 255);
	}
	void WindowsConsole::SetColor(const rgb8i &col)
	{
		fontCol = col;
		SetWindowsConsoleColor(fontCol, backCol);
	}
	void WindowsConsole::SetBackColor(const rgb8i &col)
	{
		backCol = col;
		SetWindowsConsoleColor(fontCol, backCol);
	}
	void WindowsConsole::SetBold(bool state)
	{
		isBold = state;
		if (state)
			std::cout << "\033[1m";
		else
			std::cout << "\033[2m";
	}
	void WindowsConsole::SetUnderline(bool state)
	{
		isUnderline = state;
		if (state)
			std::cout << "\033[4m";
		else
			std::cout << "\033[24m";
	}
	void WindowsConsole::SetDefault()
	{
		indentionLevel = 0;
		fontCol = rgb8i(192, 192, 192);
		backCol = rgb8i(0, 0, 0);
		isBold = false;
		isUnderline = false;
		std::cout << "\033[0m";
	}

	vec2i WindowsConsole::GetCursorPosition()
	{
		CONSOLE_SCREEN_BUFFER_INFO cbsi;
		if (GetConsoleScreenBufferInfo(HConsole(), &cbsi))
		{
			return {cbsi.dwCursorPosition.X, cbsi.dwCursorPosition.Y};
		}
		else
		{
			return vec2i(-1, -1);
		}
	}
	int WindowsConsole::GetIndention()
	{
		return indentionLevel;
	}
	rgb8i WindowsConsole::GetColor()
	{
		return fontCol;
	}
	rgb8i WindowsConsole::GetBackColor()
	{
		return backCol;
	}
	bool WindowsConsole::GetBold()
	{
		return isBold;
	}
	bool WindowsConsole::GetUnderline()
	{
		return isUnderline;
	}

	char WindowsConsole::GetLastLoggedChar()
	{
		return lastLoggedChar;
	}
	std::ostream &WindowsConsole::operator<<(std::string_view sv)
	{
		if (sv.size() == 0)
			return std::cout;

		size_t pos = sv.find('\n');
		if (pos == -1)
			pos = sv.size() - 1;
		pos++;

		std::string_view subSV(sv.begin(), sv.begin() + pos);
		if (GetLastLoggedChar() == '\n')
			std::cout << std::string(GetIndention(), ' ') << subSV;
		else
			std::cout << subSV;
		lastLoggedChar = subSV[subSV.size() - 1];

		return WindowsConsole::operator<<(std::string_view(sv.begin() + pos, sv.end()));
	}
	std::ostream &WindowsConsole::operator<<(char sv)
	{
		if (GetLastLoggedChar() == '\n')
			std::cout << std::string(GetIndention(), ' ') << sv;
		else
			std::cout << sv;
		lastLoggedChar = sv;
		return std::cout;
	}
#pragma endregion

#pragma region FileConsole

	FileConsole::FileConsole(const char *sv)
	{
		ofstream.open(sv);
	}

	void FileConsole::ShowCursor(bool show)
	{
	}
	void FileConsole::SetCursorPosition(vec2i pos)
	{
	}
	void FileConsole::SetIndention(int indention)
	{
		indentionLevel = Maths::Clamp(indention, 0, 255);
	}
	void FileConsole::SetColor(const rgb8i &col)
	{
	}
	void FileConsole::SetBackColor(const rgb8i &col)
	{
	}
	void FileConsole::SetBold(bool state)
	{
	}
	void FileConsole::SetUnderline(bool state)
	{
	}
	void FileConsole::SetDefault()
	{
	}

	vec2i FileConsole::GetCursorPosition()
	{
		return vec2i();
	}
	int FileConsole::GetIndention() { return indentionLevel; }
	rgb8i FileConsole::GetColor()
	{
		return rgb8i();
	}
	rgb8i FileConsole::GetBackColor()
	{
		return rgb8i();
	}
	bool FileConsole::GetBold()
	{
		return false;
	}
	bool FileConsole::GetUnderline()
	{
		return false;
	}

	char FileConsole::GetLastLoggedChar() { return lastLoggedChar; }
	std::ostream &FileConsole::operator<<(std::string_view sv)
	{
		if (sv.size() == 0)
			return ofstream;

		size_t pos = sv.find('\n');
		if (pos == -1)
			pos = sv.size() - 1;
		pos++;

		std::string_view subSV(sv.begin(), sv.begin() + pos);
		if (GetLastLoggedChar() == '\n')
			ofstream << std::string(GetIndention(), ' ') << subSV;
		else
			ofstream << subSV;
		lastLoggedChar = subSV[subSV.size() - 1];

		return FileConsole::operator<<(std::string_view(sv.begin() + pos, sv.end()));
	}
	std::ostream &FileConsole::operator<<(char sv)
	{
		if (GetLastLoggedChar() == '\n')
			ofstream << std::string(GetIndention(), ' ') << sv;
		else
			ofstream << sv;
		lastLoggedChar = sv;
		return ofstream;
	}
#pragma endregion
}