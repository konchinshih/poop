#pragma once

#include "logger.hpp"

#include <string>

enum struct MessageType: int {
	RAW, HELLO, DBD, LSR, LSU
};

static inline std::string toString(MessageType type)
{
	switch (type) {
	case MessageType::RAW:
		return "Raw";
	case MessageType::HELLO:
		return "Hello";
	case MessageType::DBD:
		return "DBD";
	case MessageType::LSR:
		return "LSR";
	case MessageType::LSU:
		return "LSU";
	default:
		ERROR << "type error" << std::endl;
	}
	exit(1);
}
