#include "Flag.h"
#include <iostream>
namespace Kek
{
	void FlagSet::SetUp(char flags)
	{
		value |= flags;
	}
	void FlagSet::SetDown(char flags)
	{
		value &= ~flags;
	}
	bool FlagSet::IsUp(char bits) const
	{
		return (value & bits) == bits;
	}
	bool FlagSet::IsDown(char bits) const
	{
		return (value & bits) == 0; 
	}

	FlagSet::FlagSet(char value) : value(value)
	{
	}

	FlagSet::operator char& ()
	{
		return value;
	}

	std::ostream& Kek::operator<<(std::ostream& os, const FlagSet& o)
	{
		os << int(o.value);
		return os;
	}
}
