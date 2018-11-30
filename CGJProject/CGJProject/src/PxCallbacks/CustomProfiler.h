#pragma once

#include "foundation/PxProfiler.h"

class CustomProfiler : public physx::PxProfilerCallback
{
public:

	void* zoneStart(const char* eventName, bool detached, uint64_t contextId) override;

	void zoneEnd(void* profilerData, const char* eventName, bool detached, uint64_t contextId) override;
};