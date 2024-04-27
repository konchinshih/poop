#pragma once

#include "Message.hpp"

#include <vector>
#include <istream>

using RouterId = int;

struct LinkStateRequestMessage: Message, std::vector<RouterId> {
	LinkStateRequestMessage(RouterId src = 0, RouterId dst = 0);
	std::string toString(void);
};

std::istream& operator>>(std::istream& is, LinkStateRequestMessage& msg);
