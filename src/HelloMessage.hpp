#pragma once

#include "Message.hpp"

#include <istream>

struct HelloMessage: Message {
	bool received;

	HelloMessage(RouterId src = 0, RouterId dst = 0);
	std::string toString(void) const;
};

std::istream& operator>>(std::istream& is, HelloMessage& msg);
