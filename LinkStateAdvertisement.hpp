#pragma once

#include <map>
#include <istream>

using RouterId = int;
using Cost = int;
using Sequence = int;

struct LinkStateAdvertisement: std::map<RouterId, Cost> {
	Sequence seq;

	LinkStateAdvertisement(Sequence=0);
	std::string toString(void) const;
};

std::istream& operator>>(std::istream& is, LinkStateAdvertisement& lsa);
