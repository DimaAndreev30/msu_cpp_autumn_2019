#include <iostream>
#include <cstdlib>
#include <exception>
#include <chrono>

#include "bigint.h"

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


int main (int argc, char* argv[])
{
	if (argc > 1)
	{
		try {
			const char* str = argv[1] + 1;
			if (argv[1][0] == 'i')
				std::cout << BigInt(atoi(str));
			else if (argv[1][0] == 'u')
				std::cout << BigInt((unsigned int)atol(str));
			else std::cout << BigInt(str);
		}
		catch (std::exception &err)
		{
			std::cout << err.what();
		}
	}
}
