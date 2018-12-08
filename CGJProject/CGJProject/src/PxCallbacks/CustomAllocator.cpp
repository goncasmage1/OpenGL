#include "CustomAllocator.h"

void * CustomAllocator::allocate(size_t size, const char * typeName, const char * filename, int line)
{
	void* ptr = _aligned_malloc(size, 16);
	std::lock_guard<std::mutex> lock(mAllocationsMapLock);
	mAllocations[ptr] = Allocation(size, typeName, filename, line);
	
	return ptr;
}

void CustomAllocator::deallocate(void * ptr)
{
	if (mEnableLeakDetection && ptr)
	{
		std::lock_guard<std::mutex> lock(mAllocationsMapLock);
		auto i = mAllocations.find(ptr);
		if (i == mAllocations.end())
		{
			printf("Tried to deallocate %p which was not allocated with this allocator callback.", ptr);
		}
		else
		{
			mAllocations.erase(i);
		}
	}
	_aligned_free(ptr);
}
