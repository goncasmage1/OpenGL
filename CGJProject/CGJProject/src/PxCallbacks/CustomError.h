#pragma once

#include <iostream>
#include "foundation/PxErrorCallback.h"

class CustomError : public physx::PxErrorCallback
{
public:

	void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override;
};