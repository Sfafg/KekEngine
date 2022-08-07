#pragma once
class Byte
{
	char value = 0;
public:
	void SetBit(int position, bool state)
	{
		if (state)value |= 1 << position;
		else value &= ~(1 << position);
	}
	bool GetBit(int position) const
	{
		return ((value >> position) & 1) == 1;
	}

	Byte(int value = 0) :value(value)
	{}
	Byte(Byte const& o)
	{
		value = o.value;
	}
	operator char() { return value; }
};
std::ostream& operator<<(std::ostream& os, const Byte& o)
{
	for (int i = sizeof(o) * 8 - 1; i >= 0; i--)
	{
		if (o.GetBit(i))os << '1';
		else os << '0';
	}
	return os;
}

