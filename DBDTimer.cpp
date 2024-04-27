#include "DBDTimer.hpp"
#include "Router.hpp"
#include "logger.hpp"
#include "config.hpp"

std::function<void(std::stop_token, Router&, DBDTimer&)> DBDTimer::handler = [] (
	std::stop_token stoken, Router& router, DBDTimer& timer)
{
	DEBUG << "DBDTimer::handler called" << std::endl;

	for (;;) {
		router.sendDBD(router.BROADCAST_ID);
		
		std::jthread(DBDTimer::runner, std::ref(timer)).swap(timer.timer);	
		std::unique_lock<std::mutex> lk(timer.m);
		timer.cv.wait(lk);
	}
};

std::function<void(std::stop_token, DBDTimer&)> DBDTimer::runner = [] (
	std::stop_token stoken, DBDTimer& timer)
{
	DEBUG << "DBDTimer::runner called" << std::endl;

	for (int i = 0; i < DBD_TIMER_TICK; i++) {
		if (stoken.stop_requested()) {
			DEBUG << "timer stopped on demend" << std::endl;
			timer.cv.notify_one();
			return;
		}
		std::this_thread::sleep_for(TICK);
	}
	DEBUG << "timer stopped automatically" << std::endl;
	timer.cv.notify_one();
};

DBDTimer::DBDTimer(Router& router):
	Timer(router, std::jthread(handler, std::ref(router), std::ref(*this))) {}
