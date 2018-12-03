#include "CustomAssertHandler.h"

void CustomAssertHandler::operator()(const char * exp, const char * file, int line, bool & ignore)
{
	if (ignore) return;

	std::cerr << "NvCloth ASSERT FAILURE: " << std::endl;
	std::cerr << "  file:     " << file << std::endl;
	std::cerr << "  line:     " << line << std::endl;
	std::cerr << "  expression: " << exp << std::endl;

	exit(EXIT_FAILURE);
}
