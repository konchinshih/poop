#pragma once

#include "LinkStateAdvertisement.hpp"
#include "DatabaseDescription.hpp"

#include <map>
#include <thread>

using RouterId = int;

struct Router;

struct LinkStateDatabase: std::map<RouterId, LinkStateAdvertisement> {
	std::map<RouterId, std::jthread> timer;

	LinkStateDatabase(void);
	DatabaseDescription toDatabaseDescription(void);
};
