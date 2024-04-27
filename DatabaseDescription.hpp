#pragma once

#include <istream>
#include <map>
#include <string>

using RouterId = int;
using Sequence = int;

struct DatabaseDescription: std::map<RouterId, Sequence> {
	DatabaseDescription(void);
	std::string toString(void);
};

std::istream& operator>>(std::istream& is, DatabaseDescription& dbd);
