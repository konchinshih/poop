#include "NeighborTable.hpp"
#include "logger.hpp"

NeighborTableEntry::NeighborTableEntry(
	Cost cost, LinkState state, DatabaseDescription dbd):
	cost(cost), state(state), dbd(dbd)
{}

NeighborTable::NeighborTable(void) {}

RouterId NeighborTable::findId(std::string host, int port)
{ return port - 10000; }

LinkStateAdvertisement NeighborTable::toLSA(Sequence seq) const
{
	DEBUG << "NeighborTable::toLSA called" << std::endl;

	LinkStateAdvertisement lsa(seq + 1);
	for (const auto& [id, entry] : *this)
		lsa[id] = entry.cost;
	
	return lsa;
}
