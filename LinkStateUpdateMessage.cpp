#include "LinkStateUpdateMessage.hpp"
#include "MessageType.hpp"
#include "logger.hpp"

#include <sstream>

LinkStateUpdateMessage::LinkStateUpdateMessage(RouterId src, RouterId dst):
	Message(src, dst) {}

std::string LinkStateUpdateMessage::toString(void)
{
	INFO << "LinkStateUpdateMessage::toString called" << std::endl;

	std::stringstream ss;
	ss << (int)MessageType::LSU << std::endl;
	ss << src << ' ' << dst << std::endl;

	ss << size() << std::endl;
	for (const auto& [id, lsa] : *this)
		ss << id << std::endl << lsa.toString() << std::endl;
	return ss.str();
}

std::istream& operator>>(std::istream& is, LinkStateUpdateMessage& msg)
{
	is >> msg.src >> msg.dst;

	int n; is >> n;
	while (n--) {
		RouterId id;
		LinkStateAdvertisement lsa;
		is >> id >> lsa;
		msg[id] = lsa;
	}
	return is;
}
