#pragma once

#include "DatabaseDescription.hpp"
#include "LinkStateAdvertisement.hpp"

#include <string>

using RouterId = int;
using MACAddress = int;

struct Message {
	RouterId src, dst;

	Message(RouterId src = 0, RouterId dst = 0);
	virtual std::string toString() const = 0;
};
