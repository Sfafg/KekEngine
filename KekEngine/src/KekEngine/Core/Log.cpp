#include "Log.h"

namespace Kek
{
	IConsole* Debug::Console()
	{
		static WindowsConsole wConsole;
		static IConsole* console = &wConsole;

		return console;
	}
#ifndef NDEBUG
	FlagSet& Debug::Mask()
	{
		static FlagSet mask(~0);

		return mask;
	}
#endif // DEBUG_MODE

	// Find next sequence.
	void GetNextFormatSeqence(const std::string_view& str, int& start, int& end)
	{
		start = str.find('[', start);
		if(start == -1)
		{
			start = str.size();
			end = -1;
			return;
		}
		if(start > 0)
		{
			if(str[start - 1] == '\033')
			{
				start++;
				GetNextFormatSeqence(str, start, end);
				return;
			}
		}

		end = str.find(']', start + 1) + 1;
		if(end == 0)
		{
			start = str.size();
			return;
		}
	}

	// Translate sequence to format object.
	struct FormatData
	{
		colRGB8i fontCol;
		colRGB8i backCol;
		bool isBold;
		bool isUnderline;
		unsigned char indentionLevel;

		FormatData() { Get(); }
		void Get()
		{
			fontCol = Debug::Console()->GetColor();
			backCol = Debug::Console()->GetBackColor();
			isBold = Debug::Console()->GetBold();
			isUnderline = Debug::Console()->GetUnderline();
			indentionLevel = Debug::Console()->GetIndention();
		}
		void Set()
		{
			Debug::Console()->SetColor(fontCol);
			Debug::Console()->SetBackColor(backCol);
			Debug::Console()->SetBold(isBold);
			Debug::Console()->SetUnderline(isUnderline);
			Debug::Console()->SetIndention(indentionLevel);
		}
	};

	bool isLetter(char c)
	{
		// ASCII[97, 122]
		c = tolower(c);
		return (97 <= c && c <= 122);
	}
	bool isNum(char c)
	{
		// ASCII[48, 57]
		return (48 <= c && c <= 57);
	}
	colRGB8i ExtractColor(const std::string_view& str, int& pos)
	{
		if(str[pos] == '#')
		{
			pos += 6;
			return colRGB8i(std::string_view(str.begin() + pos - 5, str.begin() + pos + 1));
		}
		if(str[pos] == '(')
		{
			pos++;
			size_t end = str.find(')', pos);
			if(end == -1) return colRGB8i(0, 0, 0);

			colRGB8i col;
			int num = 0;
			std::stringstream ss;
			for(pos; pos < end; pos++)
			{
				if(str[pos] == ',')
				{
					int a;
					ss >> a;
					col[num] = a;
					num++;
					ss.clear();
					continue;
				}
				ss << str[pos];
			}
			int a;
			ss >> a;
			col[num] = a;
			return col;
		}
		if(str[pos] == '{')
		{
			pos++;
			size_t end = str.find('}', pos);
			if(end == -1) return colRGB8i(0, 0, 0);
			if(!Debug::Console()->colorTable.isValidName(std::string(str.begin() + pos, str.begin() + end)))return colRGB8i(0, 0, 0);
			colRGB8i col = Debug::Console()->colorTable[std::string(str.begin() + pos, str.begin() + end)];
			pos = end;
			return col;
		}
		return colRGB8i(0, 0, 0);
	}
	FormatData TranslateSequence(const std::string_view& str)
	{
		if(str.size() == 0)
		{
			Debug::Console()->SetDefault();
			return FormatData();
		}

		FormatData formatData;
		for(int i = 0; i < str.size(); i++)
		{
			if(isNum(str[i]))
			{
				int start = i;
				while(i < str.size() && isNum(str[i])) i++;
				std::stringstream ss;
				ss << std::string_view(str.begin() + start, str.begin() + i);
				int a;
				ss >> a;
				formatData.indentionLevel = a;
				i--;
				continue;
			}

			switch(str[i])
			{
			case '(': case '#': case '{':
			{
				formatData.fontCol = ExtractColor(str, i);
				break;
			}
			case 'F':
			{
				i += 2;
				formatData.fontCol = ExtractColor(str, i);
				break;
			}
			case 'B':
			{
				if(i + 1 < str.size() && str[i + 1] == 'G')
				{
					i += 2;
					formatData.backCol = ExtractColor(str, i);
					break;
				}
				if(i - 1 >= 0 && str[i - 1] == '/') formatData.isBold = false;
				else formatData.isBold = true;
				break;
			}
			case '_':
			{
				if(i - 1 >= 0 && str[i] == '/') formatData.isUnderline = false;
				else formatData.isUnderline = true;
				break;
			}
			case '<':
			{
				i++;
				int start = i;
				while(i < str.size() && isNum(str[i])) i++;
				std::stringstream ss;
				ss << std::string_view(str.begin() + start, str.begin() + i);
				int a;
				ss >> a;
				formatData.indentionLevel = Maths::Clamp(formatData.indentionLevel - a, 0, 255);
				break;
			}
			case '>':
			{
				i++;
				int start = i;
				while(i < str.size() && isNum(str[i])) i++;
				std::stringstream ss;
				ss << std::string_view(str.begin() + start, str.begin() + i);
				int a;
				ss >> a;
				formatData.indentionLevel = Maths::Clamp(formatData.indentionLevel + a, 0, 255);
				break;
			}
			default:break;
			}
		}

		return formatData;
	}
	void GetScope(const std::string_view& str, int& start, int& end)
	{
		if(start >= str.size())
		{
			start = -1;
			return;
		}

		// Check if first character after all spaces is '<'.
		while(str[start] == ' ') start++;
		if(str[start] != '<')
		{
			// Return invalid scope.
			start = -1;
			return;
		}
		start++;
		// Find Scope start and end with correct hirarchy and excluding Format Sequences.
		int lCount = 1;
		int rCount = 0;
		for(int i = start; i < str.size(); i++)
		{
			switch(str[i])
			{
			case '[':
			{
				size_t index = str.find(']', i);
				if(index != -1) i = index;
			}
			continue;

			case '>':
				rCount++;
				if(rCount == lCount)
				{
					end = i;
					return;
				}
				break;

			case '<':
				lCount++;
				break;

			default:break;
			}
		}

		// Default return invalid scope.
		start = -1;
	}

	void TranslateAndLog(const std::string_view& str)
	{
		if(str.size() == 0) return;

		int start = 0, end = 0;
		GetNextFormatSeqence(str, start, end);
		*Debug::Console() << std::string_view(str.begin(), str.begin() + start);

		if(end == -1) return;

		int scopeStart = end;
		int scopeEnd = 0;
		GetScope(std::string_view(str.begin(), str.end()), scopeStart, scopeEnd);
		if(scopeStart == -1)
		{
			FormatData newFormat = TranslateSequence(std::string_view(str.begin() + start + 1, str.begin() + end - 1));
			newFormat.Set();
			TranslateAndLog(std::string_view(str.begin() + end, str.end()));
		}
		else
		{
			FormatData currentFormat;
			FormatData newFormat = TranslateSequence(std::string_view(str.begin() + start + 1, str.begin() + end - 1));
			newFormat.Set();
			TranslateAndLog(std::string_view(str.begin() + scopeStart, str.begin() + scopeEnd));
			currentFormat.Set();
			TranslateAndLog(std::string_view(str.begin() + scopeEnd + 1, str.end()));
		}
	}

	template <>
	void Log<LogType::Info>(const std::string_view& str)
	{
#ifndef NDEBUG
		if(Debug::Mask().IsDown(LogType::Info)) return;
#endif // DEBUG_MODE

		colRGB8i col = Debug::Console()->GetColor();
		Debug::Console()->SetColor("Green");
		*Debug::Console() << "Info: ";

		Debug::Console()->SetColor("LYellow");
		TranslateAndLog(str);
		Debug::Console()->SetColor(col);
		*Debug::Console() << '\n';
	}
	template <>
	void Log<LogType::Warning>(const std::string_view& str)
	{
#ifndef NDEBUG
		if(Debug::Mask().IsDown(LogType::Warning)) return;
#endif // DEBUG_MODE

		colRGB8i col = Debug::Console()->GetColor();
		Debug::Console()->SetColor("Yellow");
		*Debug::Console() << "Warning: ";

		Debug::Console()->SetColor("LYellow");
		TranslateAndLog(str);
		Debug::Console()->SetColor(col);
		*Debug::Console() << '\n';
	}
	template <>
	void Log<LogType::Error>(const std::string_view& str)
	{
#ifndef NDEBUG
		if(Debug::Mask().IsDown(LogType::Error)) return;
#endif // DEBUG_MODE

		colRGB8i col = Debug::Console()->GetColor();
		Debug::Console()->SetColor("Red");
		*Debug::Console() << "Error: ";

		Debug::Console()->SetColor("LYellow");
		TranslateAndLog(str);
		Debug::Console()->SetColor(col);
		*Debug::Console() << '\n';
	}
	template <>
	void Log<LogType::Raw>(const std::string_view& str)
	{
#ifndef NDEBUG
		if(Debug::Mask().IsDown(LogType::Raw)) return;
#endif // DEBUG_MODE

		*Debug::Console() << str;
	}
	template <>
	void Log<LogType::None>(const std::string_view& str)
	{
		TranslateAndLog(str);
		*Debug::Console() << '\n';
	}
}