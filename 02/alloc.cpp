#include <cstdint>

#include "alloc.h"


LinearAllocator::LinearAllocator (size_t maxSize):
	maxSize_ (maxSize), occupied_ (0)
{
	if (maxSize <= 0)
	{
		maxSize_ = 0;
		mem_ = nullptr;
	}
	else 
		mem_ = new char[maxSize_];
}

LinearAllocator::~LinearAllocator ()
{
	if (mem_) delete [] mem_;
}


char* LinearAllocator::alloc (size_t size)
{
	if (size >= SIZE_MAX - maxSize_) 
		throw std::bad_alloc ();

	if (occupied_ + size > maxSize_)
		return nullptr;
	else
	{
		char* ptr = mem_ + occupied_;
		occupied_ += size;
		return ptr;
	}
}


void LinearAllocator::reset ()
{
	occupied_ = 0;
}
