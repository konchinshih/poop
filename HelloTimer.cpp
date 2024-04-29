#include "HelloTimer.hpp"
#include "Router.hpp"
#include "logger.hpp"
#include "config.hpp"

std::function<void(std::stop_token, Router&, HelloTimer&)> HelloTimer::handler = [] (
	std::stop_token stoken, Router& router, HelloTimer& timer)
{
	DEBUG << "HelloTimer::handler called" << std::endl;

	for (;;) {
		std::jthread(HelloTimer::runner, std::ref(timer)).swap(timer.timer);	
		std::unique_lock lk(timer.m);
		timer.cv.wait(lk);
		
		std::unique_lock rwlk(router.m);
		router.sendHello(router.BROADCAST_ID);
	}
};

std::function<void(std::stop_token, HelloTimer&)> HelloTimer::runner = [] (
	std::stop_token stoken, HelloTimer& timer)
{
	DEBUG << "HelloTimer::runner called" << std::endl;

	std::chrono::time_point start = std::chrono::steady_clock::now();
	do {	
		std::this_thread::yield();

		if (stoken.stop_requested()) {
			DEBUG << "timer stopped on demend" << std::endl;
			timer.cv.notify_one();
			return;
		}
		std::this_thread::sleep_for(TICK);
	} while (std::chrono::steady_clock::now() < start + HELLO_TIMER);
	DEBUG << "timer stopped automatically" << std::endl;
	timer.cv.notify_one();
};

HelloTimer::HelloTimer(Router& router):
	Timer(router, std::jthread(handler, std::ref(router), std::ref(*this))) {}
