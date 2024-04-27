#include "RoutingTable.hpp"
#include "Router.hpp"
#include "logger.hpp"

#include <queue>
#include <set>

RoutingTableEntry::RoutingTableEntry(Cost cost, RouterId nextHop):
	cost(cost), nextHop(nextHop) {}

RoutingTable::RoutingTable(Router& router):
	router(router) {}

struct Link {
	RouterId id;
	Cost cost;
	Link(RouterId id, Cost cost):
		id(id), cost(cost) {}
};

bool operator<(Link a, Link b)
{ return a.cost > b.cost; }

void RoutingTable::calculate(void)
{
	INFO << "RoutingTable::calculate called" << std::endl;

	clear();

	std::priority_queue<Link> pq;
	pq.emplace(router.id, 0);
	at(router.id) = RoutingTableEntry(router.id, 0);

	std::set<int> visited;
	for (;;) {
		while (pq.size() && visited.count(pq.top().id))
			pq.pop();
		if (!pq.size())
			break;

		int a = pq.top().id;
		pq.pop();
		visited.insert(a);

		if (router.lsdb.count(a))
			for (const auto& [b, cost] : router.lsdb[a])
				if (at(a).cost + cost < at(b).cost) {
					at(b) =	RoutingTableEntry(a, at(a).cost + cost);
					pq.emplace(b, at(b).cost);
				}
	}
}
