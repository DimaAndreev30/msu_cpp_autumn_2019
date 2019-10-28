#include <iostream>
#include "matrix.h"

int main (int argc, char* argv [])
{
	const char* BOOLC [] = {"false", "true"};
	size_t rows = 5, cols = 7;

	Matrix m(rows, cols);

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			m[i][j] = (1 + i)*(1 + j);

	std::cout << "multiplication table (m):\n";
	std::cout << m << std::endl;

	Matrix m2 = m;
	std::cout << "m = m is " << BOOLC[m == m] << std::endl;
	std::cout << "copy of m = m is " << BOOLC[m2 == m] << std::endl;

 	m *= 4;
	std::cout << "4*m equals:\n";
	std::cout << m << std::endl;

	return 0;
}
