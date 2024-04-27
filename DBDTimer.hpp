#pragma once

#include "Timer.hpp"

#include <functional>
#include <stop_token>

struct Router;

struct DBDTimer: Timer {
	static std::function<
		void(std::stop_token, Router&, DBDTimer&)> handler;
	static std::function<
		void(std::stop_token, DBDTimer&)> runner;

	DBDTimer(Router&);
};
