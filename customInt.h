#pragma once
#include <iostream>
#include <vector>

/*
Custom integer class
Dynamic widths
Don't use unsigned ints because they are buggy!
*/
struct customInt
{
	//LAST BIT IS SIGN!!!
	std::vector<bool> digits;

	//Constructors
	customInt();
	customInt(int n);
	customInt(const std::string& n);
	customInt(std::size_t n, bool d);
	customInt(const customInt& customInt);

	//Destructor
	~customInt();

	//Get binary string from a customInt (variable length)
	std::string asBinary() const;
	std::string asDecimal() const;

	//Returns the sign (1 = negative, 0 = positive according to 2's complement)
	bool getSign() const;

	//Flips a bit/bits between 0 and 1
	void flip();
	void flip(std::size_t index);
	void negate();

	//Trim binary representation (not including sign padding)
	void trim();

	//Gets the value of bit at index safely
	bool getBit(std::size_t index) const;
	bool getBitDefault(std::size_t index, bool defaultSign) const;

	//Comparison operators
	bool operator>(const customInt& rhs) const;
	bool operator<(const customInt& rhs) const;
	bool operator==(const customInt& rhs) const;
	bool operator!=(const customInt& rhs) const;
	bool operator<=(const customInt& rhs) const;
	bool operator>=(const customInt& rhs) const;

	customInt operator+(customInt rhs);
	customInt operator-(customInt rhs);
	customInt operator*(customInt rhs);

	void operator+=(customInt rhs);
	void operator-=(customInt rhs);
	void operator*=(customInt rhs);

	void operator++(int);
	void operator--(int);

	//Type casts
	operator int()
	{
		int result = 0;
		int y = 1;

		for (std::size_t x = 0, s = digits.size() - 1; x < s; ++x)
		{
			if (digits[x]) result += y;
			y = y << 1;
		}

		return digits[digits.size() - 1] ? -result : result;
	}
	operator std::size_t()
	{
		int result = 0;
		int y = 1;

		for (std::size_t x = 0, s = digits.size() - 2; x < s; ++x)
		{
			if (digits[x]) result += y;
			y = y << 1;
		}

		return result;
	}
};