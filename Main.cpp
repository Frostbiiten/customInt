#include "customInt.h"
#include <cassert>
#include <string>

int main()
{
	customInt max = 100;
	for (customInt x = customInt(0) - max; x < max; x++)
	{
		customInt y = max - x;
		x.trim();
		y.trim();
		std::string xStr = x.asDecimal();
		std::string yStr = y.asDecimal();
		std::cout << xStr << " + " << yStr << " = " << (x + y).asDecimal() << '\n';
		std::cout << xStr << " - " << yStr << " = " << (x - y).asDecimal() << '\n';
		std::cout << yStr << " - " << xStr << " = " << (y - x).asDecimal() << '\n';
		std::cout << xStr << " * " << yStr << " = " << (x * y).asDecimal() << '\n';
	}

	return 0;
}