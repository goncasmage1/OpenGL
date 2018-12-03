#pragma once

#include <iostream>

#include "foundation/PxAllocatorCallback.h"

class CustomAllocator : public physx::PxAllocatorCallback
{
public:

	void* allocate(size_t size, const char* typeName, const char* filename, int line) override;

	void deallocate(void* ptr) override;
};