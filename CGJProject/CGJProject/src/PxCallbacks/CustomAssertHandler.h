#pragma once

#include <iostream>
#include "foundation/PxAssert.h"

class CustomAssertHandler : public physx::PxAssertHandler
{
public:

	void operator()(const char* exp, const char* file, int line, bool& ignore) override;
};