#include "CustomAllocator.h"

void * CustomAllocator::allocate(size_t size, const char * typeName, const char * filename, int line)
{
	std::cout << filename << " is allocating " << size << " bytes at line " << line << std::endl;

	std::allocator<void *> alloc;
	return alloc.allocate(size);
}

void CustomAllocator::deallocate(void * ptr)
{
	delete ptr;
}
