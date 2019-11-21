#include <iostream>
#include "calc.h"


int main (int argc, char* argv [])
{
	const char* expression = nullptr;

	if (argc > 1) 
	{
		expression = argv [1];
	}
	else
	{
		std::cout << "Missing input\n";

		return 1;
	}
	
	Calc calc;

	try {
		std::cout /*<< argv [1] << " = " */<< calc.calculate (expression) << '\n';
	}
	catch (CalcException& err)
	{
		std::cout << err.what () << '\n';
		std::cout << argv [1] << '\n';

		for (int i = 0; i < err.where (); i++)
		{
			std::cout << ' ';
		}

		std::cout << '^' << '\n';	
			
		return 1;
	}
	catch (std::exception& err)
	{
		std::cout << err.what () << '\n';
	}

	return 0;
}
