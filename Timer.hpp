#pragma once

#include <mutex>
#include <condition_variable>
#include <thread>

struct Router;

struct Timer: std::jthread {
	Router& router;
	std::jthread timer;
	std::mutex m;
	std::condition_variable cv;

	Timer(Router& router, std::jthread&&);
	void refresh(void);
};
