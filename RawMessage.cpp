#include "RawMessage.hpp"
#include "MessageType.hpp"
#include "logger.hpp"

#include <sstream>
#include <limits>

RawMessage::RawMessage(RouterId src, RouterId dst):
	Message(src, dst) {}

std::string RawMessage::toString(void)
{
	INFO << "RawMessage::toString called" << std::endl;

	std::stringstream ss;
	ss << (int)MessageType::RAW << std::endl;
	ss << src << ' ' << dst << std::endl;
	ss << payload;
	return ss.str();
}

std::istream& operator>>(std::istream& is, RawMessage& msg)
{
	is >> msg.src >> msg.dst;
	is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::getline(is, msg.payload);
	return is;
}
