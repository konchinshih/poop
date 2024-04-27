#include "RoutingTable.hpp"
#include "Router.hpp"
#include "logger.hpp"

#include <queue>
#include <set>

RoutingTableEntry::RoutingTableEntry(Cost cost, RouterId nextHop):
	cost(cost), nextHop(nextHop) {}

bool operator==(const RoutingTableEntry& a, const RoutingTableEntry& b)
{ return a.cost == b.cost && a.nextHop == b.nextHop; }

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
	DEBUG << "RoutingTable::calculate called" << std::endl;

	std::map tmp = *this;
	clear();

	std::priority_queue<Link> pq;
	pq.emplace(router.id, 0);
	emplace(router.id, RoutingTableEntry(0, router.id));

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
				if (!count(b) || at(a).cost + cost < at(b).cost) {
					emplace(b, RoutingTableEntry(at(a).cost + cost, a));
					pq.emplace(b, at(b).cost);
				}
	}

	for (const auto& [id, entry] : *this) {
		if (!tmp.count(id)) 
			OUTPUT << "add route " << id << ' ' 
				<< entry.nextHop << ' ' << entry.cost << std::endl;
		else if (entry != tmp[id])
			OUTPUT << "update route " << id << ' '
				<< entry.nextHop << ' ' << entry.cost << std::endl;
		tmp.erase(id);
	}
	for (const auto& [id, entry] : tmp)
		OUTPUT << "remove route " << id << std::endl;
}
