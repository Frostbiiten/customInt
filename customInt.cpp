#include "customInt.h"
#include <cmath>
#include <string>
#include <bitset>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <array>
#include <assert.h>

customInt ONE = 1;
customInt FOUR = 4;

//String trimming functions
std::string ltrim(const std::string& s)
{
	size_t start = s.find_first_not_of('0');
	return (start == std::string::npos) ? "" : s.substr(start);
}
std::string rtrim(const std::string& s)
{
	size_t end = s.find_last_not_of('0');
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
std::string trim(const std::string& s)
{
	return rtrim(ltrim(s));
}

//Checks if number is odd or even
bool oddEnd(std::string& num)
{
	if (num.size() == 0) { return false; }
	return (num[num.size() - 1] == '1' ||
		num[num.size() - 1] == '3' ||
		num[num.size() - 1] == '5' ||
		num[num.size() - 1] == '7' ||
		num[num.size() - 1] == '9');
}
bool oddEnd(char digit)
{
	return (digit == '1' ||
		digit == '3' ||
		digit == '5' ||
		digit == '7' ||
		digit == '9');
}

//Divides a string by two
std::string div2(const std::string& input)
{
	std::string num = input;
	int nextAdditive = 0;

	bool nonZero = false;

	for (std::size_t x = 0, s = num.size(); x < s; ++x)
	{
		int additive = nextAdditive;
		if (oddEnd(num[x]))
			nextAdditive = 5;
		else
			nextAdditive = 0;
		int digitInt = num[x] - '0';
		digitInt /= 2;//should floor
		digitInt += additive;

		num[x] = '0' + digitInt;
	}

	num = ltrim(num);
	//trim zeros? (not on right side because those are significant digits)

	return num;
}

//Bound-safe index access
inline char getChar(const std::string& string, int index)
{
	return (index >= 0 && index < string.length()) ? string[index] : '0';
}
std::string add(std::string a, std::string b)
{
	if (a.size() < b.size())
		swap(a, b);

	int j = a.size() - 1;
	for (int i = b.size() - 1; i >= 0; i--, j--)
		a[j] += (b[i] - '0');

	for (int i = a.size() - 1; i > 0; i--)
	{
		if (a[i] > '9')
		{
			int d = a[i] - '0';
			a[i - 1] = ((a[i - 1] - '0') + d / 10) + '0';
			a[i] = (d % 10) + '0';
		}
	}
	if (a[0] > '9')
	{
		std::string k;
		k += a[0];
		a[0] = ((a[0] - '0') % 10) + '0';
		k[0] = ((k[0] - '0') / 10) + '0';
		a = k + a;
	}
	return a;
}

#pragma region Initializers
customInt::customInt()
{	
	//create empty 2 bit number
	digits.resize(2);
}
customInt::customInt(int n)
{
	std::string binary = std::bitset<8 * sizeof(n)>(n).to_string();
	digits.reserve(binary.size());

	for (int x = binary.size() - 1; x >= 0; x--)
		digits.push_back(binary[x] == '1');

	trim();
}
customInt::customInt(const std::string& n)
{
	if (n == "0" || n == "-0" || n == "") return;

	std::string number = n;

	//???
	if (number[0] == '-')
	{
		number.erase(0, 1);

		while (number != "")
		{
			digits.push_back(oddEnd(number));
			number = div2(number);
		}

		digits.push_back(false);

		negate();
	}
	else
	{
		while (number != "")
		{
			digits.push_back(oddEnd(number));
			number = div2(number);
		}

		digits.push_back(false);
	}

	trim();
}
customInt::customInt(std::size_t n, bool d)
{
	digits.resize(n, d);
}
customInt::customInt(const customInt& customInt)
{
	digits = customInt.digits;
}
customInt::~customInt(){}
#pragma endregion

bool customInt::getSign() const
{
	return digits[digits.size() - 1];
}
inline bool isZero(customInt& num)
{
	for (std::size_t x = 0, s = num.digits.size(); x < s; ++x)
	{
		if (num.digits[x]) return false;
	}
	return true;
}

std::string customInt::asBinary() const
{
	std::string output;
	output.reserve(digits.size());
	for (int x = digits.size() - 1; x >= 0; x--)
	{
		if (digits[x])
			output += '1';
		else
			output += '0';
	}
	return output;
}
std::string customInt::asDecimal() const
{
	std::string decimal = "0";
	std::string base = "1";

	if (getSign())
	{
		auto inverse = *this;
		inverse.negate();
		for (std::size_t x = 0, s = inverse.digits.size() - 1; x < s; ++x)
		{
			if (inverse.digits[x]) decimal = add(decimal, base);
			base = add(base, base);
		}
		decimal.insert(decimal.begin(), '-');
	}
	else
	{
		for (std::size_t x = 0, s = digits.size() - 1; x < s; ++x)
		{
			if (digits[x]) decimal = add(decimal, base);
			base = add(base, base);
		}
	}

	return decimal;
}

void customInt::flip()
{
	for (std::size_t x = 0, s = digits.size(); x < s; ++x)
	{
		digits[x] = !digits[x];
	}
}
void customInt::flip(std::size_t index)
{
	digits[index] = !digits[index];
}
void customInt::negate()
{
	flip();
	customInt custom1{digits.size(), false};
	custom1.digits[0] = 1;
	*this = *this + custom1;
}
void customInt::trim()
{
	std::size_t trimBegin = digits.size() - 2;
	
	if (getSign())
	{
		for (std::size_t x = digits.size() - 1; x-- > 0;)
		{
			if (!digits[x])
			{
				trimBegin = x + 1;
				break;
			}
		}
	}
	else
	{
		for (std::size_t x = digits.size() - 1; x-- > 0;)
		{
			if (digits[x])
			{
				trimBegin = x + 1;
				break;
			}
		}
	}

	digits.erase(digits.begin() + trimBegin, digits.end() - 1);
}
bool customInt::getBit(std::size_t index) const
{
	if (getSign())
	{
		if (index < digits.size()) return digits[index];
		else return true;
	}
	else
	{
		if (index < digits.size()) return digits[index];
		else return false;
	}
}
bool customInt::getBitDefault(std::size_t index, bool defaultSign) const
{
	if (index < digits.size()) return digits[index];
	else return defaultSign;
}

bool customInt::operator>(const customInt& rhs) const
{
	//If one is positive and one is negative
	if (getSign() ^ rhs.getSign()) return !getSign();

	if (getSign())
	{
		//check which number has more digits in binary
		if (rhs.digits.size() > digits.size())
		{
			//iterate through the larger one, starting from end - 1 (because the last bit is the sign, starts from most significant digits)
			for (int x = rhs.digits.size() - 2; x >= 0; x--)
			{
				//if a digit on the lhs is larger, return true. In the opposite case, return false.
				if (getBit(x) && !rhs.digits[x])
					return true;
				else if (!getBit(x) && rhs.digits[x])
					return false;
			}
		}
		else
		{
			//runs through the same actions described above, but with swapped numbers*
			for (int x = digits.size() - 2; x >= 0; x--)
			{
				if (digits[x] && !rhs.getBit(x))
					return true;
				else if (!digits[x] && rhs.getBit(x))
					return false;
			}
		}
	}
	else
	{
		//check which number has more digits in binary
		if (rhs.digits.size() > digits.size())
		{
			//iterate through the larger one, starting from end - 1 (because the last bit is the sign, starts from most significant digits)
			for (int x = rhs.digits.size() - 2; x >= 0; x--)
			{
				//if a digit on the lhs is larger, return true. In the opposite case, return false.
				if (getBit(x) && !rhs.digits[x])
					return true;
				else if (!getBit(x) && rhs.digits[x])
					return false;
			}
		}
		else
		{
			//runs through the same actions described above, but with swapped numbers*
			for (int x = digits.size() - 2; x >= 0; x--)
			{
				if (digits[x] && !rhs.getBit(x))
					return true;
				else if (!digits[x] && rhs.getBit(x))
					return false;
			}
		}
	}

	return false;
}
bool customInt::operator<(const customInt& rhs) const
{
	//If one is positive and one is negative
	if (getSign() ^ rhs.getSign()) return getSign();

	if (getSign())
	{
		//check which number has more digits in binary
		if (rhs.digits.size() > digits.size())
		{
			//iterate through the larger one, starting from end - 1 (because the last bit is the sign, starts from most significant digits)
			for (int x = rhs.digits.size() - 2; x >= 0; x--)
			{
				//if a digit on the lhs is larger, return true. In the opposite case, return false.
				if (getBit(x) && !rhs.digits[x])
					return false;
				else if (!getBit(x) && rhs.digits[x])
					return true;
			}
		}
		else
		{
			//runs through the same actions described above, but with swapped numbers*
			for (int x = digits.size() - 2; x >= 0; x--)
			{
				if (digits[x] && !rhs.getBit(x))
					return false;
				else if (!digits[x] && rhs.getBit(x))
					return true;
			}
		}
	}
	else
	{
		//check which number has more digits in binary
		if (rhs.digits.size() > digits.size())
		{
			//iterate through the larger one, starting from end - 1 (because the last bit is the sign, starts from most significant digits)
			for (int x = rhs.digits.size() - 2; x >= 0; x--)
			{
				//if a digit on the lhs is larger, return true. In the opposite case, return false.
				if (getBit(x) && !rhs.digits[x])
					return false;
				else if (!getBit(x) && rhs.digits[x])
					return true;
			}
		}
		else
		{
			//runs through the same actions described above, but with swapped numbers*
			for (int x = digits.size() - 2; x >= 0; x--)
			{
				if (digits[x] && !rhs.getBit(x))
					return false;
				else if (!digits[x] && rhs.getBit(x))
					return true;
			}
		}
	}

	return false;
}
bool customInt::operator==(const customInt& rhs) const
{
	if (rhs.digits.size() > digits.size())
	{
		for (std::size_t x = 0, s = rhs.digits.size(); x < s; ++x)
		{
			if (rhs.digits[x] != getBit(x))
				return false;
		}

		return true;
	}
	else
	{
		for (std::size_t x = 0, s = rhs.digits.size(); x < s; ++x)
		{
			if (rhs.getBit(x) != digits[x])
				return false;
		}
		return true;
	}
}
bool customInt::operator!=(const customInt& rhs) const
{
	if (rhs.digits.size() > digits.size())
	{
		for (std::size_t x = 0, s = rhs.digits.size(); x < s; ++x)
		{
			if (rhs.digits[x] != getBit(x))
				return true;
		}

		return false;
	}
	else
	{
		for (std::size_t x = 0, s = digits.size(); x < s; ++x)
		{
			if (rhs.getBit(x) != digits[x])
				return true;
		}

		return false;
	}
}
bool customInt::operator<=(const customInt& rhs) const
{
	return (*this == rhs || *this < rhs);
}
bool customInt::operator>=(const customInt& rhs) const
{
	return (*this == rhs || *this > rhs);
}

customInt customInt::operator+(customInt rhs)
{
	bool c = false;

	/*lhs and rhs must have the same width*/
	if (rhs.digits.size() > digits.size())
	{
		/*Inserts extra bit to prevent overflow*/
		rhs.digits.push_back(rhs.digits[rhs.digits.size() - 1]);

		bool sign = digits[digits.size() - 1];

		/*Insert bits to make the sizes equal*/
		digits.resize(rhs.digits.size(), sign);
	}
	else if (rhs.digits.size() < digits.size())
	{
		/*Inserts extra bit to prevent overflow*/
		digits.push_back(digits[digits.size() - 1]);

		/*Removes and remembers sign*/
		bool sign = rhs.digits[rhs.digits.size() - 1];

		/*Insert bits to make the sizes equal*/
		rhs.digits.resize(digits.size(), sign);
	}
	else
	{
		//Insert extra bit to prevent overflow
		digits.push_back(digits[digits.size() - 1]);
		rhs.digits.push_back(rhs.digits[rhs.digits.size() - 1]);
	}

	customInt result{digits.size(), false};

	for (std::size_t x = 0, s = digits.size(); x < s; ++x)
	{
		result.digits[x] = ((digits[x] != rhs.digits[x]) != c);
		c = ((digits[x] && rhs.digits[x]) || (digits[x] && c)) || (rhs.digits[x] && c);
	}

	return result;
}
customInt customInt::operator-(customInt rhs)
{
	rhs.negate();
	return *this + rhs;
}
customInt discardAdd(const customInt& a, const customInt& b)
{
	customInt result{a.digits.size(), false};
	bool c = false;
	for (std::size_t x = 0, s = a.digits.size(); x < s; ++x)
	{
		result.digits[x] = ((a.digits[x] != b.digits[x]) != c);
		c = ((a.digits[x] && b.digits[x]) || (a.digits[x] && c)) || (b.digits[x] && c);
	}
	return result;
}

customInt customInt::operator*(customInt rhs)
{
	/*INIT*/
	/*lhs and rhs must have the same width*/
	if (rhs.digits.size() > digits.size())
	{
		/*Insert bits to make the sizes equal*/
		digits.resize(rhs.digits.size(), digits[digits.size() - 1]);
	}
	else if(rhs.digits.size() < digits.size())
	{
		/*Insert bits to make the sizes equal*/
		rhs.digits.resize(digits.size(), rhs.digits[digits.size() - 1]);
	}

	customInt neg = rhs;
	neg.negate();

	customInt reg{rhs.digits.size() + digits.size(), false};

	for (std::size_t x = 0, s = digits.size(); x < s; ++x)
	{
		reg.digits[x] = digits[x];
	}

	/*The final bit beside the product register*/
	bool finalBit = false;

	/*Iterates for every bit in the number*/
	for (std::size_t x = 0, s = rhs.digits.size(); x < s; ++x)
	{
		/*Only executes if both bits have different values.
		If they are the same, it hops directly to the shift*/
		if (finalBit != reg.digits[0])
		{
			//Grab final n bits *MAKE CUSTOM CONSTRUCTOR for range init*
			customInt finalBits;
			finalBits.digits.clear();
			finalBits.digits.reserve(rhs.digits.size());
			finalBits.digits.insert(finalBits.digits.begin(), reg.digits.end() - rhs.digits.size(), reg.digits.end()); // - 1 here maybe

			if (!finalBit)
			{
				/* 10 - Subtract */
				finalBits = discardAdd(finalBits, neg);
			}
			else
			{
				/* 01 - Add*/
				finalBits = discardAdd(finalBits, rhs);
			}

			/*Replace the calculated bits*/
			for (std::size_t x = 0, s = rhs.digits.size(); x < s; ++x)
			{
				reg.digits[reg.digits.size() - 1 - x] = finalBits.digits[finalBits.digits.size() - 1 - x];
			}
		}

		/*Arithmetic shift at end of every iteration*/
		finalBit = reg.digits[0];
		bool rBit = reg.digits[reg.digits.size() - 1];
		std::rotate(reg.digits.begin(), reg.digits.begin() + 1, reg.digits.end());
		reg.digits[reg.digits.size() - 1] = rBit;
	}

	return reg;
}

void customInt::operator+=(customInt rhs)
{
	*this = *this + rhs;
}
void customInt::operator-=(customInt rhs)
{
	*this = *this - rhs;
}
void customInt::operator*=(customInt rhs)
{
	*this = *this * rhs;
}

void customInt::operator++(int)
{
	*this = *this + ONE;
}
void customInt::operator--(int)
{
	*this = *this - ONE;
}
