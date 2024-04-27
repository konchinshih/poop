#include "misc.hpp"

#include <iostream>
#include <cstdlib>

void printHelp(void)
{
	std::cerr << "usage: ./ospf RouterId" << std::endl;
	exit(1);
}

std::string getHost(void)
{
	return "127.0.0.1";
}

int getPort(RouterId id)
{
	return id + 10000;
}
