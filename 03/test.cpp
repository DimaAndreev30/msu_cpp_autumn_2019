#include <iostream>
#include "matrix.h"


#define check_equal(x, y) { \
	if ((x) != (y)) std::cout << "line " << __LINE__ << ": expected " << (y) << " got " << (x) << std::endl; \
	else std::cout << "pass\n"; \
}

#define check_cond(COND) { \
	if (!(COND)) std::cout << "line " << __LINE__ << ": failed \"" << #COND << "\"\n"; \
	else std::cout << "pass\n"; \
}

#define check_throw(expr, err) do { \
	try { expr; } \
	catch (const err& arr) { std::cout << "pass\n"; break; } \
	catch (...) {} \
	std::cout << "line " << __LINE__ << ": expected " << #err << std::endl; \
} while(0);



int main ()
{
	size_t rows = 5, cols = 10;

	Matrix m(rows, cols);
	Matrix m_(0, 0);

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			m[i][j] = i*j;

	check_equal(m[0][0], 0)
	check_equal(m[1][1], 1)
	check_equal(m[4][7], 28)
	
	Matrix m2 = m;
	m[3][3] = -10;

	check_equal(m[3][3], -10)
	check_equal(m2[3][3], 9)

	check_equal(m.getRows(), rows)
	check_equal(m.getCols(), cols)
	check_equal(m_.getRows(), 0)
	check_equal(m_.getCols(), 0)

	const Matrix m3 = m;
	const Matrix m4(rows + 1, cols);
	const Matrix& m5 = m;

	check_cond(m == m)
	check_cond(m != m2)
	check_cond(m == m3)
	check_cond(m != m4)
	check_cond(m == m5)

	check_throw(m[rows][0], std::out_of_range)
	check_throw(m[0][cols], std::out_of_range)
	check_throw(m[-1][0], std::out_of_range)
	check_throw(m[0][-1], std::out_of_range)
	check_throw(m_[0][0], std::out_of_range)

	m *= 5;
	m_ *= 5;
	check_equal(m[2][3], 30)


	return 0;
}
