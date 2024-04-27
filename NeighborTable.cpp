#include "NeighborTable.hpp"

NeighborTableEntry::NeighborTableEntry(
	std::string host, int port, 
	Cost cost, LinkState state, DatabaseDescription dbd):
	host(host), port(port), cost(cost), state(state), dbd(dbd)
{}

NeighborTable::NeighborTable(void) {}

RouterId NeighborTable::findId(std::string host, int port)
{ return port - 10000; }
