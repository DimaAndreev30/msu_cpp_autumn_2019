#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <exception>


template<class T>
void collect_args(std::vector<std::string>& arr, const T& arg)
{
	std::stringstream stream;
	stream << arg;
	arr.push_back(stream.str());
}

template<class T, class... ArgsT>
void collect_args(std::vector<std::string>& arr, const T& arg, const ArgsT&... args)
{
	std::stringstream stream;
	stream << arg;
	arr.push_back(stream.str());

	collect_args(arr, args...);
}


std::string format(const char* str)
{
	return str;
}

template<class... ArgsT>
std::string format(const char* str, const ArgsT&... args)
{
	std::vector<std::string> arr;
	collect_args(arr, args...);

	std::stringstream stream;

	for (; *str; str++)
	{
		if (*str != '{')
		{
			if (*str != '}') stream << *str;
			else throw std::runtime_error("extra close brace");
		}
		else
		{
			str++;

			if (std::isdigit(*str))
			{
				uint8_t index = *str - '0';
				for (str++; std::isdigit(*str); str++)
					index = 10*index + (*str - '0');

				if (*str == '}')
				{
					if (index >= arr.size())
						throw std::runtime_error("invalid argument");

					stream << arr[index];
				}
				else throw std::runtime_error("missing close brace");
			}
			else throw std::runtime_error("missing argument index");
		}
	}

	return stream.str();
}
