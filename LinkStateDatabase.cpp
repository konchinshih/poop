#include "LinkStateDatabase.hpp"
#include "logger.hpp"
#include "Router.hpp"

LinkStateDatabase::LinkStateDatabase(Router& router): router(router) {}

DatabaseDescription LinkStateDatabase::toDatabaseDescription(void)
{
	INFO << "LinkStateDatabase::toDatabaseDescription called" << std::endl;
	
	DatabaseDescription dbd;
	for (const auto& [id, lsa] : *this)
		dbd[id] = lsa.seq;
	return dbd;
}

void LinkStateDatabase::update(RouterId src, LinkStateAdvertisement lsa)
{
	INFO << "LinkStateDatabase::update called" << std::endl;
	
	if (lsa.seq <= at(src).seq) 
		return;
	at(src) = lsa;

	router.sendLSU(Router::BROADCAST_ID);
}
