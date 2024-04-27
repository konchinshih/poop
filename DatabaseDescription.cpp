#include "DatabaseDescription.hpp"
#include "logger.hpp"

#include <sstream>

DatabaseDescription::DatabaseDescription(void) {}

std::string DatabaseDescription::toString(void)
{
	INFO << "DatabaseDescription::toString called" << std::endl;
	std::ostringstream ss;
	ss << size() << std::endl;
	for (const auto& [id, seq] : *this)
		ss << id << ' ' << seq << std::endl;
	return ss.str();
}

std::istream& operator>>(std::istream& is, DatabaseDescription& dbd)
{
	int n; is >> n;
	while(n--) {
		RouterId id;
		Sequence seq;
		is >> id >> seq;
		dbd[id] = seq;
	}
	return is;
}
