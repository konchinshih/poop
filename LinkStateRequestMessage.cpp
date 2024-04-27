#include "LinkStateRequestMessage.hpp"
#include "MessageType.hpp"
#include "logger.hpp"

#include <sstream>

LinkStateRequestMessage::LinkStateRequestMessage(RouterId src, RouterId dst):
	Message(src, dst) {}

std::string LinkStateRequestMessage::toString(void)
{
	INFO << "LinkStateRequestMessage::toString called" << std::endl;

	std::stringstream ss;
	ss << (int)MessageType::LSR << std::endl;
	ss << src << ' ' << dst << std::endl;
	
	for (const auto& id : *this)
		ss << id << ' ';
	return ss.str();
}

std::istream& operator>>(std::istream& is, LinkStateRequestMessage& msg)
{
	is >> msg.src >> msg.dst;
	
	int n; is >> n;
	msg.resize(n);
	for (auto& id : msg)
		is >> id;
	return is;
}
