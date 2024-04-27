#pragma once

#include "Message.hpp"

#include <string>
#include <istream>

struct DatabaseDescriptionMessage: Message {
	DatabaseDescription dbd;

	DatabaseDescriptionMessage(RouterId src = 0, RouterId dst = 0);
	std::string toString(void);
};

std::istream& operator>>(std::istream& is, DatabaseDescriptionMessage& msg);
