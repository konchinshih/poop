#include "Timer.hpp"
#include "Router.hpp"
#include "logger.hpp"

#include <utility>

Timer::Timer(Router& router, std::jthread&& j): 
	router(router), std::jthread(std::move(j)) {}

void Timer::refresh(void)
{
	DEBUG << "Timer::refresh called" << std::endl;

	timer.request_stop();
}
