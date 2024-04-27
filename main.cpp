#include "Router.hpp"
#include "logger.hpp"
#include "misc.hpp"

#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[])
{
	INFO << "main called" << std::endl;

	if (argc <= 1)
		printHelp();
	int id = atoi(argv[1]);

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
