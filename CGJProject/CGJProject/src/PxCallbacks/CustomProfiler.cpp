#include "CustomProfiler.h"

void * CustomProfiler::zoneStart(const char * eventName, bool detached, uint64_t contextId)
{
	return nullptr;
}

void CustomProfiler::zoneEnd(void * profilerData, const char * eventName, bool detached, uint64_t contextId)
{
}
