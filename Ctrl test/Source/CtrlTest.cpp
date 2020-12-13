#include <iostream>
#include "Logger.h"

int main()
{
	std::cout << "Hello World" << std::endl;

	LOG_FATAL_PUSH("We've hit a fatal error");


	return 0;
}