#include <iostream>
#include "matrix.h"


std::ostream& operator<<(std::ostream& out, const Matrix& m)
{
	for (int i = 0; i < m.getRows(); i++)
	{
		for (int j = 0; j < m.getCols(); j++)
		{
			out << m[i][j] << ' ';
		}
		
		out << std::endl;
	}

	return out;
}
