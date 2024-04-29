#include "Router.hpp"
#include "logger.hpp"
#include "misc.hpp"

#include <iostream>
#include <cstdlib>
#include <cstring>

bool verbose = false;

int main(int argc, char *argv[])
{
	INFO << "main called" << std::endl;

	if (argc <= 1)
		printHelp();
	int id = atoi(argv[1]);

	if (argc > 2 && strcmp(argv[2], "-v") == 0) 
		verbose = true;

	Router router(id);
	router.listen(getHost(), getPort(id));

	std::string input;
	while(getline(std::cin, input)) {
		if (input == "exit")
			break;
		router.command(input);
	}

	return 0;
}
