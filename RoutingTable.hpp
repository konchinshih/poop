#pragma once

#include <map>

using RouterId = int;
using Cost = int;

struct Router;

struct RoutingTableEntry {
	Cost cost;
	RouterId nextHop;

	RoutingTableEntry(Cost cost = 0, RouterId nextHop = 0);
};

struct RoutingTable: std::map<RouterId, RoutingTableEntry> {
	Router& router;
	RoutingTable(Router& router);

	void calculate(void);
};
