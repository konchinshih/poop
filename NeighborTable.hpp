#pragma once

#include "DatabaseDescription.hpp"
#include "LinkState.hpp"

#include <map>
#include <string>

using RouterId = int;
using Cost = int;

struct NeighborTableEntry {
	std::string host;
	int port;

	Cost cost;
	LinkState state;
	DatabaseDescription dbd;

	NeighborTableEntry(
		std::string host = "127.0.0.1",
		int port = 10000,
		Cost cost = 0,
		LinkState state = LinkState::DOWN,
		DatabaseDescription dbd = DatabaseDescription()
	);
};

struct NeighborTable: std::map<RouterId, NeighborTableEntry> {
	NeighborTable();

	RouterId findId(std::string host, int port);
};
