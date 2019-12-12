#include <iostream>
#include <fstream>

#include <cstdint>

#include "sort.h"

int main (int argc, char* argv[])
{
	const char* input_filename = "input.txt";
	const char* output_filename = "output.txt";
	if (--argc > 0) input_filename = *(++argv);
	if (--argc > 0) output_filename = *(++argv);

	std::ifstream input (input_filename, std::ios::in | std::ios::binary);
	std::ofstream output (output_filename, std::ios::out | std::ios::binary);
	if (input && output)
	{
		sort(input, output, 1024);
		output.close();
		input.close();

		input.open(output_filename, std::ios::in | std::ios::binary);
		int n = 0;
		uint64_t prev = -0xFFFFFFFFFFFFFFFF, next = 0;
		while(!input.read(reinterpret_cast<char*>(&next), sizeof(uint64_t)).eof())
		{
			n++;
			if (prev > next) std::cout << prev << " > " << next << std::endl;
		}
		std::cout << n << std::endl;
		input.close();
	}
	else
	{
		std::cout << "File \'" << input_filename << "\' not found\n";
	}

	return 0;/*
	int n = 8192;
	const char* output_filename = "input.txt";
	if (--argc > 0) n = atoi(*(++argv));
	if (--argc > 0) output_filename = *(++argv);
	std::ofstream output (output_filename, std::ios::out | std::ios::binary);

	for (int i = 0; i < n; i++)
	{
		uint64_t a = rand();
		output.write(reinterpret_cast<char*>(&a), sizeof(uint64_t));
	}
	output.close();*//*
	const char* input_filename = "input.txt";
	if (--argc > 0) input_filename = *(++argv);
	std::ifstream input(input_filename, std::ios::in | std::ios::binary);

	int n = 0;
	uint64_t a = 0;
	while(!input.read(reinterpret_cast<char*>(&a), sizeof(uint64_t)).eof())
	{
		std::cout << a << std::endl;
		n++;
	}
	std::cout << n;
	input.close();*/
}
