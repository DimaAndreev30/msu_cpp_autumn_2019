#include <iostream>
#include <cstdio>

#include <cstdint>
#include "alloc.h"


int main (int argc, char* argv [])
{
	if (argc > 1)
	{
		try {
			LinearAllocator man (atoi (argv[1]));

			char *mem = man.alloc (1), *ptr;
			man.reset ();

			for (int i = 2; i < argc; i++)
			{
				switch (argv[i][0])
				{
				  case 'a':
					ptr = man.alloc (atoi (argv[i] + 1));
					if (ptr == nullptr)
						std::cout << -1 << std::endl;
					else
						std::cout << ptr - mem << std::endl;
					break;

				  case 'r':
					man.reset ();
					break;

				  default:
					i = argc;
				}
			}
		}
		catch (std::exception &err)
		{
			std::cout << err.what ();
			return 1;
		}

		std::cout << "All_right\n";

	}
	else
	{
		std::cout << "Missing_input\n";
	}

	return 0;
}
