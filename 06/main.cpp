#include <iostream>

#include "format.h"


int main (int argc, char* argv[])
{
	int a = 1;
	int b = 6;
	const char* str = "asdfasdf";
	std::cout << format("lal: {0}, lol: {1}, lel: {2}, lul: {1}, lil: {0}", a, 4, str) << std::endl;
}
