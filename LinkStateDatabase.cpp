#include "LinkStateDatabase.hpp"
#include "logger.hpp"
#include "Router.hpp"
#include "config.hpp"

std::function<void(std::stop_token, Router&, RouterId, 
	std::chrono::steady_clock::time_point)> LinkStateDatabase::runner = [] (
	std::stop_token stoken, Router& router, RouterId id, 
	std::chrono::steady_clock::steady_clock::time_point expire)
{
	std::unique_lock lk(router.m);
	auto sleep = std::min(expire, std::chrono::steady_clock::now() + LSA_EXPIRE);
	lk.unlock();

	do {
		std::this_thread::yield();

		if (stoken.stop_requested())
			return;

		std::this_thread::sleep_for(TICK);
	} while (std::chrono::steady_clock::now() < sleep);
	
	lk.lock();
	INFO << "lsdb LSA id " << id << " expired" << std::endl;
	OUTPUT << "remove LSA " << id << std::endl;
	router.lsdb.erase(id);
};


LinkStateDatabase::LinkStateDatabase(Router& router): 
	router(router) {}

DatabaseDescription LinkStateDatabase::toDatabaseDescription(void)
{
	DEBUG << "LinkStateDatabase::toDatabaseDescription called" << std::endl;
	
	DatabaseDescription dbd;
	for (const auto& [id, lsa] : *this)
		dbd[id] = lsa.seq;
	return dbd;
}

void LinkStateDatabase::refresh(RouterId id)
{ std::jthread(runner, std::ref(router), id, at(id).expire).swap(timer[id]); }
