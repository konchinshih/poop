#pragma once

#include "LinkStateAdvertisement.hpp"
#include "DatabaseDescription.hpp"

#include <map>
#include <thread>

using RouterId = int;

struct Router;

struct LinkStateDatabase: std::map<RouterId, LinkStateAdvertisement> {
	Router& router;
	std::map<RouterId, std::jthread> timer;

	LinkStateDatabase(Router& router);
	DatabaseDescription toDatabaseDescription(void);
	void update(RouterId src, LinkStateAdvertisement lsa);
};
