#pragma once

#include "DatabaseDescription.hpp"
#include "LinkState.hpp"
#include "LinkStateAdvertisement.hpp"

#include <map>
#include <string>
#include <thread>
#include <functional>

using RouterId = int;
using MACAddress = int;
using Cost = int;
using Sequence = int;

struct Router;

struct NeighborTableEntry {
	MACAddress mac;
	Cost cost;
	LinkState state;
	DatabaseDescription dbd;

	NeighborTableEntry(
		Cost cost = 65535,
		LinkState state = LinkState::DOWN,
		DatabaseDescription dbd = DatabaseDescription()
	);
};

struct NeighborTable: std::map<RouterId, NeighborTableEntry> {
	Router& router;
	static std::function<void(std::stop_token,
		Router&, RouterId)> runner;
	std::map<RouterId, std::jthread> timer;

	std::map<MACAddress, RouterId> mac;

	NeighborTable(Router&);
	LinkStateAdvertisement toLSA(Sequence) const;

	void refresh(RouterId);
};
