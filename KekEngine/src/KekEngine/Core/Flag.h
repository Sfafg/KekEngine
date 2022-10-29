#pragma once
#include <iosfwd>

namespace Kek
{
	struct FlagSet
	{
		void SetUp(char flags);
		void SetDown(char flags);
		bool IsUp(char flags) const;
		bool IsDown(char flags) const;

		FlagSet(char value);
		operator char& ();

		private:
		char value;
		friend std::ostream& operator<<(std::ostream& os, const FlagSet& o);
	};

	std::ostream& operator<<(std::ostream& os, const FlagSet& o);
}