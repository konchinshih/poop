#pragma once

#include <map>
#include <istream>
#include <chrono>

using RouterId = int;
using Cost = int;
using Sequence = int;

struct LinkStateAdvertisement: std::map<RouterId, Cost> {
	static Sequence cur;
	Sequence seq;
	std::chrono::steady_clock::time_point expire;

	LinkStateAdvertisement(int increasement = 0);
	std::string toString(void) const;
};

std::istream& operator>>(std::istream& is, LinkStateAdvertisement& lsa);
