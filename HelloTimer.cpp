#include "HelloTimer.hpp"
#include "Router.hpp"
#include "logger.hpp"
#include "config.hpp"

std::function<void(std::stop_token, Router&, HelloTimer&)> HelloTimer::handler = [] (
	std::stop_token stoken, Router& router, HelloTimer& timer)
{
	DEBUG << "HelloTimer::handler called" << std::endl;

	for (;;) {
		router.sendHello(router.BROADCAST_ID);
		
		std::jthread(HelloTimer::runner, std::ref(timer)).swap(timer.timer);	
		std::unique_lock<std::mutex> lk(timer.m);
		timer.cv.wait(lk);
	}
};

std::function<void(std::stop_token, HelloTimer&)> HelloTimer::runner = [] (
	std::stop_token stoken, HelloTimer& timer)
{
	DEBUG << "HelloTimer::runner called" << std::endl;

	for (int i = 0; i < HELLO_TIMER_TICK; i++) {
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

HelloTimer::HelloTimer(Router& router):
	Timer(router, std::jthread(handler, std::ref(router), std::ref(*this))) {}
