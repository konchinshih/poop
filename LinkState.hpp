#pragma once

#include "logger.hpp"

#include <string>

enum struct LinkState: int {
	DOWN, INIT, EXCHANGE, FULL
};

static inline std::string toString(LinkState state)
{
	switch (state) {
	case LinkState::DOWN:
		return "Down";
	case LinkState::INIT:
		return "Init";
	case LinkState::EXCHANGE:
		return "Exchange";
	case LinkState::FULL:
		return "Full";
	default:
		ERROR << "state error" << std::endl;
	}
	exit(1);
}
