#include "Router.hpp"
#include "MessageType.hpp"
#include "logger.hpp"
#include "misc.hpp"

#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

Router::Router(int id): id(id), lsdb(*this), rt(*this), 
	helloTimer(*this), lsaTimer(*this) {}

void Router::listen(std::string host, int port)
{
	INFO << "Router::listen called" << std::endl;
	INFO << "listen on " << host << ":" << port << std::endl;

	sockaddr_in addr;
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		ERROR << "cannot create socket" << std::endl;
		exit(1);
	}

	memset((void *)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	if (inet_pton(AF_INET, host.data(), &addr.sin_addr) <= 0) {
		ERROR << "cannot parse hostname" << std::endl;
		exit(1);
	}
	addr.sin_port = htons(port);

	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		ERROR << "cannot bind udp port" << std::endl;
		exit(1);
	}

	std::jthread([](int fd, Router& router) {
		INFO << "listener started" << std::endl;

		struct sockaddr_in addr;
		socklen_t addrlen = sizeof(addr);
		char buf[RECV_BUFSIZE + 1];
		int recvlen;

		for (;;) {
			recvlen = recvfrom(fd, buf, RECV_BUFSIZE, 0, (struct sockaddr *)&addr, &addrlen);
			if (recvlen < 0) {
				ERROR << "listen port failed" << std::endl;
				exit(1);
			}
			if (recvlen > 0) {
				INFO << "receive something" << std::endl;
				buf[recvlen] = '\0';
				
				char tmp[100];
				std::string host(inet_ntop(AF_INET, &addr.sin_addr, tmp, 100));
				int port = ntohs(addr.sin_port);
				DEBUG << "host, port = " << host << ',' << port << std::endl;
				router.parse(buf);
			}
		}
		ERROR << "listener dies" << std::endl; 
	}, fd, std::ref(*this)).swap(listener);
}

void Router::send(RouterId id, std::string content)
{
	INFO << "Router::send called" << std::endl;
	INFO << "send to RouterId = " << id << std::endl;

	int fd = socket(AF_INET, SOCK_DGRAM, 0);

	auto sendOne = [&](RouterId dst) {
		DEBUG << "sendOne called" << std::endl;

		sockaddr_in addr;
		bzero(&addr, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(getHost().data());
		addr.sin_port = htons(getPort(id));
		
		int sent = 0, sendlen;
//		do {
			sendlen = sendto(fd, 
					content.substr(sent).data(), content.size() - sent, 
					0, (struct sockaddr *)&addr, sizeof(addr));

			if (sendlen < 0) {
				ERROR << "cannot send content" << std::endl;
				exit(1);
			}
//		} while(sendlen > 0);

		DEBUG << "sendOne ended" << std::endl;
	};
	
	if (id == Router::BROADCAST_ID)
		for (const auto& [i, entry] : nt)
			sendOne(i);
	else
		sendOne(id);

	close(fd);
}

void Router::command(std::string input)
{
	INFO << "Router::command called" << std::endl;
	INFO << "input = " << input << std::endl;

	std::stringstream ss(input);
	
	std::string op; ss >> op;
	if (op == "addlink") {
		RouterId dst;
		Cost cost;
		ss >> dst >> cost;
		nt[dst] = NeighborTableEntry(
			getHost(), getPort(dst), cost		
		);
	} // TODO: add more commands
}

void Router::parse(std::string content)
{
	INFO << "Router::parse called" << std::endl;
	
	std::stringstream ss(content);
	int type; ss >> type;
	switch((MessageType)type) {
#define MESSAGE_CASE(id, Type, handler) \
	case id: { \
		Type msg; \
		ss >> msg; \
		handler(msg); \
		break; \
	}
	MESSAGE_CASE(MessageType::RAW, RawMessage, handleRaw)
	MESSAGE_CASE(MessageType::HELLO, HelloMessage, handleHello)
	MESSAGE_CASE(MessageType::DBD, DatabaseDescriptionMessage, handleDBD)
	MESSAGE_CASE(MessageType::LSR, LinkStateRequestMessage, handleLSR)
	MESSAGE_CASE(MessageType::LSU, LinkStateUpdateMessage, handleLSU)
#undef MESSAGE_CASE
	}
}

void Router::handleRaw(RawMessage msg)
{

}

void Router::handleHello(HelloMessage msg)
{
	INFO << "Router::handleHello called" << std::endl;
	
	if (nt[msg.src].state == LinkState::DOWN) {
		if (msg.received)
			nt[msg.src].state = LinkState::EXCHANGE;
		else
			nt[msg.src].state = LinkState::INIT;
	} else if (nt[msg.src].state == LinkState::INIT) {
		if (msg.received)
			nt[msg.src].state = LinkState::EXCHANGE;
	}
	INFO << "nt[msg.src].state = " << (int)nt[msg.src].state << std::endl;
}

void Router::handleDBD(DatabaseDescriptionMessage msg)
{

}

void Router::handleLSR(LinkStateRequestMessage msg)
{

}

void Router::handleLSU(LinkStateUpdateMessage msg)
{
	
}

void Router::sendRaw(RouterId dst)
{

}

void Router::sendHello(RouterId dst)
{
	INFO << "Router::sendHello called" << std::endl;

	if (dst == Router::BROADCAST_ID)
		for (const auto& [i, entry] : nt) {
			HelloMessage msg(this->id, i);
			msg.received = (entry.state != LinkState::DOWN);
			this->send(i, msg.toString());
		}
	else
		this->send(dst, HelloMessage(this->id, dst).toString());
}

void Router::sendDBD(RouterId routerId)
{

}

void Router::sendLSR(RouterId routerId)
{

}

void Router::sendLSU(RouterId routerId)
{

}
