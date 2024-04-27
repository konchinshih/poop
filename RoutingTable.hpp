#pragma once

#include <map>

using RouterId = int;
using Cost = int;

struct Router;

struct RoutingTableEntry {
	Cost cost;
	RouterId nextHop;

	RoutingTableEntry(Cost cost, RouterId nextHop);
};

struct RoutingTable: std::map<RouterId, RoutingTableEntry> {
	Router& router;
	RoutingTable(Router& router);

	void calculate(void);
};
