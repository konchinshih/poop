#pragma once

#include <string>

using RouterId = int;

void printHelp(void);

std::string getHost(void);

int getPort(RouterId);
RouterId parsePort(int);
