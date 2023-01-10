#include "Byte.h"
#include <iostream>
namespace Kek
{
	void Byte::Write(int position, bool state)
	{
		if (state)
			Byte::Set(position);
		else
			Byte::Unset(position);
	}
	void Byte::Set(int position)
	{
		value = (1 << position) | value;
	}
	void Byte::Unset(int position)
	{
		value &= ~(1 << position);
	}
	void Byte::Toggle(int position)
	{
		value ^= 1 << position;
	}
	bool Byte::Get(int position) const
	{
		return (value >> position & 1) == 1;
	}
	Byte::Byte(unsigned char value) : value(value)
	{
	}
	Byte::operator unsigned char &()
	{
		return value;
	}

	std::ostream &operator<<(std::ostream &os, const Byte &o)
	{
		os << "Byte(";
		for (int i = 7; i >= 0; i--)
		{
			os << o.Get(i);
		}
		os << ')';
		return os;
	}
}