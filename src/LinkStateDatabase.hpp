#pragma once

#include "LinkStateAdvertisement.hpp"
#include "DatabaseDescription.hpp"

#include <map>
#include <thread>
#include <functional>
#include <stop_token>
#include <chrono>
#include <shared_mutex>

using RouterId = int;

struct Router;

struct LinkStateDatabase: std::map<RouterId, LinkStateAdvertisement> {
	Router& router;
	static std::function<void(std::stop_token, 
			Router&, RouterId, std::chrono::steady_clock::time_point)> runner;
	std::map<RouterId, std::jthread> timer;

	LinkStateDatabase(Router&);
	DatabaseDescription toDatabaseDescription(void);

	void refresh(RouterId);
};
