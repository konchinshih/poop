#include "HelloMessage.hpp"
#include "MessageType.hpp"
#include "logger.hpp"

#include <sstream>

HelloMessage::HelloMessage(RouterId src, RouterId dst):
	Message(src, dst) {}

std::string HelloMessage::toString(void) const
{
	DEBUG << "HelloMessage::toString called" << std::endl;

	std::stringstream ss;
	ss << (int)MessageType::HELLO << std::endl;
	ss << src << ' ' << dst << std::endl;
	ss << received;
	return ss.str();
}

std::istream& operator>>(std::istream& is, HelloMessage& msg)
{
	is >> msg.src >> msg.dst;
	is >> msg.received;
	return is;
}
