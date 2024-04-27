#include "LSATimer.hpp"
#include "Router.hpp"
#include "logger.hpp"
#include "config.hpp"

std::function<void(std::stop_token, Router&, LSATimer&)> LSATimer::handler = [] (
	std::stop_token stoken, Router& router, LSATimer& timer)
{
	DEBUG << "LSATimer::handler called" << std::endl;

	for (;;) {
		// TODO: LSA refresh

		std::jthread(LSATimer::runner, std::ref(timer)).swap(timer.timer);
		std::unique_lock<std::mutex> lk(timer.m);
		timer.cv.wait(lk);
	}
};

std::function<void(std::stop_token, LSATimer&)> LSATimer::runner = [] (
	std::stop_token stoken, LSATimer& timer)
{
	DEBUG << "LSATimer::runner called" << std::endl;

	for (int i = 0; i < LSA_TIMER_TICK; i++) {
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

LSATimer::LSATimer(Router& router):
	Timer(router, std::jthread(handler, std::ref(router), std::ref(*this))) {}
