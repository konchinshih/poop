#pragma once

#include "Message.hpp"

#include <istream>
#include <string>

struct RawMessage: Message {
	std::string payload;

	RawMessage(RouterId src = 0, RouterId dst = 0);
	std::string toString(void);
};

std::istream& operator>>(std::istream& is, RawMessage& msg);
