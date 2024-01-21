#include "Error.h"
#include <iostream>

void Error(const char* message)
{
	std::cout <<"ERROR:: "<< message << std::endl;
	std::cin.get();
	exit(EXIT_FAILURE);
}

void Log(const char* message)
{
	std::cout << "OK:: " << message << std::endl;
}