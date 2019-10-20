#pragma once

#include <cstddef>
#include <exception>

class LinearAllocator
{
	size_t maxSize_;
	char* mem_;
	size_t occupied_;

  public:
	LinearAllocator(size_t maxSize);
	~LinearAllocator();

	char* alloc(size_t size);
	void reset();
};
