#pragma once

#include "Timer.hpp"

#include <functional>
#include <stop_token>

struct Router;

struct HelloTimer: Timer {
	static std::function<
		void(std::stop_token, Router&, HelloTimer&)> handler;
	static std::function<
		void(std::stop_token, HelloTimer&)> runner;

	HelloTimer(Router&);
};
