#include "LinkStateDatabase.hpp"
#include "logger.hpp"
#include "Router.hpp"

LinkStateDatabase::LinkStateDatabase() {}

DatabaseDescription LinkStateDatabase::toDatabaseDescription(void)
{
	INFO << "LinkStateDatabase::toDatabaseDescription called" << std::endl;
	
	DatabaseDescription dbd;
	for (const auto& [id, lsa] : *this)
		dbd[id] = lsa.seq;
	return dbd;
}

