#pragma once

#include <chrono>

constexpr std::chrono::duration TICK = std::chrono::milliseconds(50);
constexpr std::chrono::duration LSA_EXPIRE = std::chrono::seconds(30);
constexpr std::chrono::duration HELLO_TIMER = std::chrono::seconds(1);
constexpr std::chrono::duration DBD_TIMER = std::chrono::seconds(1);
constexpr std::chrono::duration LSA_TIMER = std::chrono::seconds(15);
constexpr std::chrono::duration DOWN_TIMER = 4 * HELLO_TIMER;
