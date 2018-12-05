#pragma once

#include <iostream>
#include <map>
#include <mutex>

#include "foundation/PxAllocatorCallback.h"

class CustomAllocator : public physx::PxAllocatorCallback
{
private:

	bool mEnableLeakDetection;
	struct Allocation
	{
		Allocation() {}
		Allocation(size_t size, const char* typeName, const char* filename, int line)
			: mSize(size), mTypeName(typeName), mFileName(filename), mLine(line)
		{

		}
		size_t mSize;
		std::string mTypeName;
		std::string mFileName;
		int mLine;
	};
	std::map<void*, Allocation> mAllocations;
	std::mutex mAllocationsMapLock;

public:

	void* allocate(size_t size, const char* typeName, const char* filename, int line) override;

	void deallocate(void* ptr) override;
};