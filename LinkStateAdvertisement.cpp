#include "LinkStateAdvertisement.hpp"
#include "logger.hpp"

#include <sstream>

LinkStateAdvertisement::LinkStateAdvertisement(Sequence seq): seq(seq) {}

std::string LinkStateAdvertisement::toString(void) const
{
	INFO << "LinkStateAdvertisement::toString called" << std::endl;

	std::stringstream ss;
	ss << seq << std::endl;
	
	ss << size() << std::endl;
	for (const auto& [id, cost] : *this)
		ss << id << ' ' << cost << std::endl;
	return ss.str();
}

std::istream& operator>>(std::istream& is, LinkStateAdvertisement& lsa)
{
	is >> lsa.seq;
	int n; is >> n;
	while (n--) {
		RouterId id;
		Cost cost;
		is >> id >> cost;
		lsa[id] = cost;
	}
	return is;
}
