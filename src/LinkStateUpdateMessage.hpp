#pragma once

#include "Message.hpp"
#include "LinkStateAdvertisement.hpp"

#include <map>
#include <istream>

using RouterId = int;

struct LinkStateUpdateMessage: Message, std::map<RouterId, LinkStateAdvertisement> {
	LinkStateUpdateMessage(RouterId src = 0, RouterId dst = 0);
	std::string toString(void) const;
};

std::istream& operator>>(std::istream& is, LinkStateUpdateMessage& msg);
