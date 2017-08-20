#include "ExString.h"
#include <iostream>

int main(int argc, char* argv[])
{
	ExStringA string;
	string.add("Hola ");
	string.add("Mundo");
	std::cout << string.c_string() << std::endl;
	string[10] = 's';
	std::cout << string << std::endl;
	return 0;
}
