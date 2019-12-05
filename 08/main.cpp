#include <iostream>

#include "pool.h"


struct A { int x; };
void foo(const A& a) { std::cerr << a.x << std::endl; }

int main (int argc, char* argv[])
{
	ThreadPool pool(8);

	auto task1 = pool.exec(foo, A{1});
	task1.get();

	auto task2 = pool.exec([]() { return 1; });
	std::cerr << task2.get() << std::endl;
}
