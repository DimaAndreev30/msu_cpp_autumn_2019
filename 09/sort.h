#pragma once

#include <fstream>
#include <cstddef>


void sort(std::ifstream& input, std::ofstream& output, size_t max_mem=0x800000);
