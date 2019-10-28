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

	Matrix m1(rows, cols);
	Matrix m2(rows, cols);
	Matrix m3(rows + 1, cols);
	Matrix m4(rows, cols);
	const Matrix& m5 = m1;
	Matrix m6(0, 0);

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			m1[i][j] = m2[i][j] = i*j;
	m1[3][3] = -10;

	check_equal(m1[0][0], 0)
	check_equal(m1[1][1], 1)
	check_equal(m1[4][7], 28)
	check_equal(m1[3][3], -10)
	check_equal(m2[3][3], 9)

	check_equal(m1.getRows(), rows)
	check_equal(m1.getCols(), cols)
	check_equal(m6.getRows(), 0)
	check_equal(m6.getCols(), 0)

	check_cond(m1 == m1)
	check_cond(m5 == m1)
	check_cond(m1 != m2)
	check_cond(m1 != m3)
	check_cond(m1 != m4)

	check_throw(m1[rows][0], std::out_of_range)
	check_throw(m1[0][cols], std::out_of_range)
	check_throw(m1[-1][0], std::out_of_range)
	check_throw(m1[0][-1], std::out_of_range)
	check_throw(m6[0][0], std::out_of_range)

	return 0;
}
