#pragma once

#include <iostream>
#include <fstream>
#include <mutex>

static constexpr char ANSI_ESC_RED[] = "\x1B[31m";
static constexpr char ANSI_ESC_GREEN[] = "\x1B[32m";
static constexpr char ANSI_ESC_YELLOW[] = "\x1B[33m";
static constexpr char ANSI_ESC_BLUE[] = "\x1B[34m";
static constexpr char ANSI_ESC_MAGENTA[] = "\x1B[35m";
static constexpr char ANSI_ESC_CYAN[] = "\x1B[36m";
static constexpr char ANSI_ESC_COLOR_RESET[] = "\x1B[m";

static std::ofstream stdnull("/dev/null");

#define LOGLEVEL 2
#define LOG(level, message, color) \
	( level <= LOGLEVEL ? \
	 	std::cout << color << message << ANSI_ESC_COLOR_RESET << " " : \
		std::cerr << color << message << ANSI_ESC_COLOR_RESET << " " )

#define DEBUG LOG(3, "[debug]", ANSI_ESC_CYAN)
#define INFO  LOG(2, "[info]", ANSI_ESC_GREEN)
#define ERROR LOG(0, "[error]", ANSI_ESC_RED)
