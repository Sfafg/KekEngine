#pragma once
#include <string_view>
#include <fstream>
#include "KekEngine/Maths/Color.h"

namespace Kek
{
	class ColorTable
	{
	public:
		static bool isValidName(const std::string &str);
		rgb8i operator[](const std::string &str);
		rgb8i operator[](int ind);
	};

	class IConsole
	{
	public:
		static ColorTable colorTable;

		/// <summary>
		/// White, Black, Blue, Green, Cyan, Red, Magenta, Yellow, Gray, LGray, LBlue, LGreen, LCyan, LRed, LMagenta, LYellow
		/// </summary>
		/// <param name="colorName"></param>
		void SetColor(const std::string &colorName);
		/// <summary>
		/// White, Black, Blue, Green, Cyan, Red, Magenta, Yellow, Gray, LGray, LBlue, LGreen, LCyan, LRed, LMagenta, LYellow
		/// </summary>
		/// <param name="colorName"></param>
		void SetBackColor(const std::string &colorName);

		virtual void ShowCursor(bool show) = 0;
		virtual void SetCursorPosition(vec2i pos) = 0;
		virtual void SetIndention(int indention) = 0;
		virtual void SetColor(const rgb8i &col) = 0;
		virtual void SetBackColor(const rgb8i &col) = 0;
		virtual void SetBold(bool state) = 0;
		virtual void SetUnderline(bool state) = 0;
		virtual void SetDefault() = 0;

		virtual vec2i GetCursorPosition() = 0;
		virtual int GetIndention() = 0;
		virtual rgb8i GetColor() = 0;
		virtual rgb8i GetBackColor() = 0;
		virtual bool GetBold() = 0;
		virtual bool GetUnderline() = 0;

		virtual char GetLastLoggedChar() = 0;
		virtual std::ostream &operator<<(std::string_view sv) = 0;
		virtual std::ostream &operator<<(char sv) = 0;
	};

	class WindowsConsole : public IConsole
	{
		rgb8i fontCol = rgb8i(192, 192, 192);
		rgb8i backCol = rgb8i(0, 0, 0);
		bool isBold = false;
		bool isUnderline = false;
		unsigned char indentionLevel = 0;
		char lastLoggedChar = '\n';

	public:
		WindowsConsole() {}

		void ShowCursor(bool show);
		void SetCursorPosition(vec2i pos);
		void SetIndention(int indention);
		void SetColor(const rgb8i &col);
		void SetBackColor(const rgb8i &col);
		void SetBold(bool state);
		void SetUnderline(bool state);
		void SetDefault();

		vec2i GetCursorPosition();
		int GetIndention();
		rgb8i GetColor();
		rgb8i GetBackColor();
		bool GetBold();
		bool GetUnderline();

		char GetLastLoggedChar();
		std::ostream &operator<<(std::string_view sv);
		std::ostream &operator<<(char sv);
	};
	class FileConsole : public IConsole
	{
		std::ofstream ofstream;
		unsigned char indentionLevel = 0;
		char lastLoggedChar = '\n';

	public:
		FileConsole(const char *sv);

		void ShowCursor(bool show);
		void SetCursorPosition(vec2i pos);
		void SetIndention(int indention);
		void SetColor(const rgb8i &col);
		void SetBackColor(const rgb8i &col);
		void SetBold(bool state);
		void SetUnderline(bool state);
		void SetDefault();

		vec2i GetCursorPosition();
		int GetIndention();
		rgb8i GetColor();
		rgb8i GetBackColor();
		bool GetBold();
		bool GetUnderline();

		char GetLastLoggedChar();
		std::ostream &operator<<(std::string_view sv);
		std::ostream &operator<<(char sv);
	};
}