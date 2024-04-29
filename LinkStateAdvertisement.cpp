#include "LinkStateAdvertisement.hpp"
#include "logger.hpp"
#include "config.hpp"

#include <sstream>

Sequence LinkStateAdvertisement::cur = 0;

LinkStateAdvertisement::LinkStateAdvertisement(int increasement): 
	seq(cur += increasement), expire(std::chrono::steady_clock::now() + LSA_EXPIRE) {}

std::string LinkStateAdvertisement::toString(void) const
{
	DEBUG << "LinkStateAdvertisement::toString called" << std::endl;

	std::stringstream ss;
	ss << seq << ' ' << expire.time_since_epoch().count() << std::endl;
	
	ss << size() << std::endl;
	for (const auto& [id, cost] : *this)
		ss << id << ' ' << cost << std::endl;
	return ss.str();
}

std::istream& operator>>(std::istream& is, LinkStateAdvertisement& lsa)
{
	std::chrono::steady_clock::rep x;
	is >> lsa.seq >> x;
	lsa.expire = std::chrono::steady_clock::time_point() 
		+ std::chrono::steady_clock::duration(x);
	int n; is >> n;
	while (n--) {
		RouterId id;
		Cost cost;
		is >> id >> cost;
		lsa[id] = cost;
	}
	return is;
}
