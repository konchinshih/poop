#pragma once

#include "DatabaseDescription.hpp"
#include "LinkState.hpp"
#include "LinkStateAdvertisement.hpp"

#include <map>
#include <string>

using RouterId = int;
using Cost = int;
using Sequence = int;

struct NeighborTableEntry {
	Cost cost;
	LinkState state;
	DatabaseDescription dbd;

	NeighborTableEntry(
		Cost cost = 0,
		LinkState state = LinkState::DOWN,
		DatabaseDescription dbd = DatabaseDescription()
	);
};

struct NeighborTable: std::map<RouterId, NeighborTableEntry> {
	NeighborTable();

	RouterId findId(std::string host, int port);
	LinkStateAdvertisement toLSA(Sequence) const;
};
