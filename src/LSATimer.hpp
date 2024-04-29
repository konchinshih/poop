#pragma once

#include "Timer.hpp"

#include <functional>
#include <stop_token>

struct Router;

struct LSATimer: Timer {
	static std::function<
		void(std::stop_token, Router&, LSATimer&)> handler;
	static std::function<
		void(std::stop_token, LSATimer&)> runner;

	LSATimer(Router& router);
};
