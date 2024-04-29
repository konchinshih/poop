#include "LSATimer.hpp"
#include "Router.hpp"
#include "logger.hpp"
#include "config.hpp"

std::function<void(std::stop_token, Router&, LSATimer&)> LSATimer::handler = [] (
	std::stop_token stoken, Router& router, LSATimer& timer)
{
	DEBUG << "LSATimer::handler called" << std::endl;

	for (;;) {
		std::jthread(LSATimer::runner, std::ref(timer)).swap(timer.timer);
		std::unique_lock lk(timer.m);
		timer.cv.wait(lk);
		
		std::unique_lock rwlk(router.m);
		router.lsdb[router.id] = router.nt.toLSA(+1);
		router.lsdb.refresh(router.id);
		OUTPUT << "update LSA " << router.id << ' ' << router.lsdb[router.id].seq << std::endl;
		
		router.sendLSU(router.id, Router::BROADCAST_ID, std::vector<RouterId>{router.id});
		router.rt.calculate();
	}
};

std::function<void(std::stop_token, LSATimer&)> LSATimer::runner = [] (
	std::stop_token stoken, LSATimer& timer)
{
	DEBUG << "LSATimer::runner called" << std::endl;

	std::chrono::time_point start = std::chrono::steady_clock::now();
	do {
		std::this_thread::yield();

		if (stoken.stop_requested()) {
			DEBUG << "timer stopped on demend" << std::endl;
			timer.cv.notify_one();
			return;
		}

		std::this_thread::sleep_for(TICK);
	} while (std::chrono::steady_clock::now() < start + LSA_TIMER);
	DEBUG << "timer stopped automatically" << std::endl;
	timer.cv.notify_one();
};

LSATimer::LSATimer(Router& router):
	Timer(router, std::jthread(handler, std::ref(router), std::ref(*this))) {}
