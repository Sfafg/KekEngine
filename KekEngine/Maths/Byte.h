#pragma once
struct Byte
{
	void Write(int position, bool state);
	void Set(int position);
	void Unset(int position);
	void Toggle(int position);
	bool Get(int position) const;

	Byte(unsigned char value = 0);
	operator unsigned char& ();

	private:
	unsigned char value = 0;
	friend std::ostream& operator<<(std::ostream& os, const Byte& o);
};

std::ostream& operator<<(std::ostream& os, const Byte& o);