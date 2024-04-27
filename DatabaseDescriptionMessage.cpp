#include "DatabaseDescriptionMessage.hpp"
#include "MessageType.hpp"
#include "logger.hpp"

#include <sstream>

DatabaseDescriptionMessage::DatabaseDescriptionMessage(RouterId src, RouterId dst):
	Message(src, dst) {}

std::string DatabaseDescriptionMessage::toString(void) const
{
	DEBUG << "DatabaseDescriptionMessage::toString called" << std::endl;
	std::stringstream ss;
	ss << (int)MessageType::DBD << std::endl;
	ss << src << ' ' << dst << std::endl;

	ss << dbd.toString();
	return ss.str();
}

std::istream& operator>>(std::istream& is, DatabaseDescriptionMessage& msg)
{
	is >> msg.src >> msg.dst;

	return is >> msg.dbd;
}
