#include "CustomAllocator.h"

void * CustomAllocator::allocate(size_t size, const char * typeName, const char * filename, int line)
{
	std::cout << filename << " is allocating " << size << " bytes at line " << line << std::endl;

	void* ptr = _aligned_malloc(size, 16);
	return ptr;
}

void CustomAllocator::deallocate(void * ptr)
{
	_aligned_free(ptr);
}
