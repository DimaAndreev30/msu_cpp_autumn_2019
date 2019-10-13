#include <iostream>
#include "calc.h"


int main (int argc, char* argv [])
{
	if (argc < 2) 
	{
		std::cout << "Missing input\n";
		return 1;
	}
	else
	{
		Calc calc = Calc ();
		if (calc.calculate (argv [1]))
		{
			std::cout /*<< argv [1] << " = " */<< calc.getResult () << '\n';
		}
		else 
		{
			calc.printErrorDescription ();
			return 1;
		}
	}

	return 0;
}
