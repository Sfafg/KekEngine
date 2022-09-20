#include "Log.h"

WindowsConsole wConsole;
IConsole* Debug::console = &wConsole;

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
		fontCol = Debug::console->GetColor();
		backCol = Debug::console->GetBackColor();
		isBold = Debug::console->GetBold();
		isUnderline = Debug::console->GetUnderline();
		indentionLevel = Debug::console->GetIndention();
	}
	void Set()
	{
		Debug::console->SetColor(fontCol);
		Debug::console->SetBackColor(backCol);
		Debug::console->SetBold(isBold);
		Debug::console->SetUnderline(isUnderline);
		Debug::console->SetIndention(indentionLevel);
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
		int end = str.find(')', pos);
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
		int end = str.find('}', pos);
		if(end == -1) return colRGB8i(0, 0, 0);
		if(!Debug::console->colorTable.isValidName(std::string(str.begin() + pos, str.begin() + end)))return colRGB8i(0, 0, 0);
		colRGB8i col = Debug::console->colorTable[std::string(str.begin() + pos, str.begin() + end)];
		pos = end;
		return col;
	}
	return colRGB8i(0, 0, 0);
}
FormatData TranslateSequence(const std::string_view& str)
{
	if(str.size() == 0)
	{
		Debug::console->SetDefault();
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
			int index = str.find(']', i);
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
	*Debug::console << std::string_view(str.begin(), str.begin() + start);

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
	colRGB8i col = Debug::console->GetColor();
	Debug::console->SetColor("Green");
	*Debug::console << "Info: ";


	Debug::console->SetColor("LYellow");
	TranslateAndLog(str);
	Debug::console->SetColor(col);
	*Debug::console << '\n';
}
template <>
void Log<LogType::Warning>(const std::string_view& str)
{
	colRGB8i col = Debug::console->GetColor();
	Debug::console->SetColor("Yellow");
	*Debug::console << "Warning: ";

	Debug::console->SetColor("LYellow");
	TranslateAndLog(str);
	Debug::console->SetColor(col);
	*Debug::console << '\n';
}
template <>
void Log<LogType::Error>(const std::string_view& str)
{
	colRGB8i col = Debug::console->GetColor();
	Debug::console->SetColor("Red");
	*Debug::console << "Error: ";

	Debug::console->SetColor("LYellow");
	TranslateAndLog(str);
	Debug::console->SetColor(col);
	*Debug::console << '\n';
}
template <>
void Log<LogType::Raw>(const std::string_view& str)
{
	*Debug::console << str << '\n';
}
template <>
void Log<LogType::None>(const std::string_view& str)
{
	TranslateAndLog(str);
	*Debug::console << '\n';
}