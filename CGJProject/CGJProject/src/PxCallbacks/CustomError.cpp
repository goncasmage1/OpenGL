#include "CustomError.h"

void CustomError::reportError(physx::PxErrorCode::Enum code, const char * message, const char * file, int line)
{
	std::cerr << "NvCloth ERROR: " << code << std::endl;
	std::cerr << "  file:     " << file << std::endl;
	std::cerr << "  line:     " << line << std::endl;
	std::cerr << "  message: " << message << std::endl;
}
