#include <iostream>
#include <cstdlib>
#include <exception>
#include <chrono>

#include <cstdint>

//#include "bigint.h"

/*
class Timer
{
    using clock_t = std::chrono::high_resolution_clock;
    using microseconds = std::chrono::microseconds;
public:
    Timer()
        : start_(clock_t::now())
    {
    }

    ~Timer()
	{
        const auto finish = clock_t::now();
        const auto us =
            std::chrono::duration_cast<microseconds>
                (finish - start_).count();
        std::cout << us << " us" << std::endl;
    }

private:
    const clock_t::time_point start_;
};*/


template <class T = uint8_t, class L = uint16_t, size_t t_size=sizeof(T)>
class A
{
	static_assert((T)(~T{}) < (L)(~L{}));

	T _x;

public:
	A (int x=0): _x(x) {}

	void print() { }
};


int main (int argc, char* argv[])
{
}
