#include "NeighborTable.hpp"
#include "Router.hpp"
#include "logger.hpp"
#include "config.hpp"

NeighborTableEntry::NeighborTableEntry(
	Cost cost, LinkState state, DatabaseDescription dbd):
	cost(cost), state(state), dbd(dbd)
{}


std::function<void(std::stop_token, Router&, RouterId)> NeighborTable::runner = [] (
	std::stop_token stoken, Router& router, RouterId id)
{
	std::chrono::time_point start = std::chrono::steady_clock::now();
	do {
		std::this_thread::yield();

		if (stoken.stop_requested())
			return;

		std::this_thread::sleep_for(TICK);
	} while (std::chrono::steady_clock::now() < start + DOWN_TIMER);

	std::unique_lock lk(router.m);
	router.nt[id].state = LinkState::DOWN;
	OUTPUT << "update neighbor state " << id << " Down" << std::endl;
};

NeighborTable::NeighborTable(Router& router): router(router) {}

LinkStateAdvertisement NeighborTable::toLSA(int increasement) const
{
	DEBUG << "NeighborTable::toLSA called" << std::endl;

	LinkStateAdvertisement lsa(increasement);
	for (const auto& [id, entry] : *this)
		lsa[id] = entry.cost;
	
	return lsa;
}

void NeighborTable::refresh(RouterId id)
{ std::jthread(runner, std::ref(router), id).swap(timer[id]); }
