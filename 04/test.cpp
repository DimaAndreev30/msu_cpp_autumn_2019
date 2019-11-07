#include <iostream>
#include "bigint.h"


#define check_cond(COND) { \
	if (!(COND)) std::cout << "line " << __LINE__ << ": failed \"" << #COND << "\"\n"; \
	else std::cout << "pass\n"; \
}


int main ()
{
	std::cout << "Операции сравнения:\n";
	check_cond(BigInt(0) == BigInt(0))
	check_cond(BigInt(1) != BigInt(0))
	check_cond(BigInt(0) != BigInt(-1))
	check_cond(BigInt(123456789) == BigInt(123456789))
	check_cond(BigInt(-45321234) != BigInt(-45321233))
	check_cond(BigInt(-666666) == BigInt(-666666))
	check_cond(BigInt("ABCDEF0101") != BigInt("ABC"))
	check_cond(BigInt("ABC") != BigInt("ABCDEF0101"))
	check_cond(BigInt("-ABCDEF") != BigInt("ABC"))
	check_cond(BigInt("ABCDEF") != BigInt("-FEDCBA"))
	check_cond(BigInt("-ABCDEF") != BigInt("-FEDCBA"))
	check_cond(BigInt("ABC123EFBBBBDD") == BigInt("ABC123EFBBBBDD"))

	std::cout << "Неравенства:\n";
	check_cond(BigInt(1) >= BigInt(0))
	check_cond(BigInt(0) >= BigInt(0))
	check_cond(BigInt(0) <= BigInt(0))
	check_cond(BigInt(0) <= BigInt(1))
	check_cond(BigInt(0) < BigInt(1))
	check_cond(BigInt(1) > BigInt(0))
	check_cond(BigInt(12345) >= BigInt(432))
	check_cond(BigInt("AFDE") >= BigInt(-123))
	check_cond(BigInt("AFDE") >= BigInt("AD12"))
	check_cond(BigInt("-12312") <= BigInt(0))
	check_cond(BigInt("ABCDEF") >= BigInt("ABCDEF"))
	check_cond(BigInt("ABCDF0") >= BigInt("ABCDEF"))

	std::cout << "Арифметические операции:\n";
	BigInt a(0),b(0),i(0);

	a = 1;
	b = -1;
	check_cond(a + b == 0)
	check_cond(a - b == 2)
	check_cond(a + -b == 2)
	check_cond(-a + b == -2)
	check_cond(b - a == -2)
	a += 1;
	check_cond(a == 2)
	a -= 1;
	check_cond(a == 1)
	a += -1;
	check_cond(a == 0)
	a -= -1;
	check_cond(a == 1)

	check_cond(BigInt(15) + BigInt("FFFFFFF0") == "FFFFFFFF")
	check_cond(BigInt("FFFFFFF0") + BigInt(15) == "FFFFFFFF")
	check_cond(BigInt(-15) + BigInt("FFFFFFFF") == "FFFFFFF0")
	check_cond(BigInt(15) + BigInt("-FFFFFFFF") == "-FFFFFFF0")
	check_cond(BigInt(-15) + BigInt("-FFFFFFF0") == "-FFFFFFFF")

	check_cond(BigInt(15) - BigInt("FFFFFFFF") == "-FFFFFFF0")
	check_cond(BigInt("FFFFFFFF") - BigInt(15) == "FFFFFFF0")
	check_cond(BigInt(-15) - BigInt("FFFFFFF0") == "-FFFFFFFF")
	check_cond(BigInt(15) - BigInt("-FFFFFFF0") == "FFFFFFFF")
	check_cond(BigInt(-15) - BigInt("-FFFFFFFF") == "FFFFFFF0")

	i = 0;
	a = 256;
	b = "1000000";
	while(a != b)
	{
		a += a;
		i += 1;
	}
	check_cond(i == 16)
}
