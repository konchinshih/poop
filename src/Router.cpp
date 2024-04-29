#include "Router.hpp"
#include "MessageType.hpp"
#include "logger.hpp"
#include "misc.hpp"

#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <shared_mutex>
#include <mutex>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

Router::Router(int id): mac(id), id(id), lsdb(*this), rt(*this), nt(*this),
	helloTimer(*this), lsaTimer(*this), dbdTimer(*this) 
{
	std::unique_lock lk(m);

	OUTPUT << "add LSA " << id << ' ' << lsdb[id].seq << std::endl;
}

void Router::listen(const std::string& host, int port)
{
	DEBUG << "Router::listen called" << std::endl;
	INFO << "listen on " << host << ":" << port << std::endl;

	sockaddr_in addr;
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		ERROR << "cannot create socket" << std::endl;
		exit(1);
	}

	memset((void *)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
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
			recvlen = recvfrom(fd, buf, RECV_BUFSIZE, 
					MSG_WAITALL, (struct sockaddr *)&addr, &addrlen);
			if (recvlen < 0) {
				ERROR << "listen port failed" << std::endl;
				exit(1);
			}
			if (recvlen > 0) {
				DEBUG << "receive something" << std::endl;
				buf[recvlen] = '\0';
				
				char tmp[100];
				std::string host(inet_ntop(AF_INET, &addr.sin_addr, tmp, 100));
				int port = ntohs(addr.sin_port);
				DEBUG << "host, port = " << host << ", " << port << std::endl;

				router.parse(buf);
			}
		}
		ERROR << "listener dies" << std::endl; 
	}, fd, std::ref(*this)).swap(listener);
}

void Router::send(RouterId id, const std::string& content)
{
	DEBUG << "Router::send called" << std::endl;
	DEBUG << "send to RouterId = " << id << std::endl;
	
	int fd = socket(AF_INET, SOCK_DGRAM, 0);

	auto sendOne = [&](RouterId dst) {
		DEBUG << "sendOne called" << std::endl;

		if (dst != this->id && !nt.count(dst))
			return;

		sockaddr_in addr;
		bzero(&addr, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(getPort(dst));

		std::string header = std::format("{} {}\n", 
				this->mac, (dst == this->id ? this->mac : nt.mac[dst]));
		std::string packet = header + content;
		
		int sendlen;
//		do {
		sendlen = sendto(fd, packet.data(), packet.size(),
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
	else {
		if (nt.count(id))
			sendOne(id);
		else {
			sendOne(rt[id].nextHop);
		}
	}

	close(fd);
}

void Router::command(const std::string& input)
{
	DEBUG << "Router::command called" << std::endl;
	DEBUG << "input = " << input << std::endl;

	std::stringstream ss(input);
	
	std::string op; ss >> op;
	if (op == "addlink" || op == "setlink") {
		RouterId dst;
		Cost cost;
		ss >> dst >> cost;

		if (op == "addlink") 
			OUTPUT << "add neighbor " << dst << ' ' << cost << std::endl;
		if (op == "setlink")
			OUTPUT << "update neighbor " << dst << ' ' << cost << std::endl;
	
		std::unique_lock lk(m);
		
		if (nt.count(dst))
			nt[dst].cost = cost;
		else
			nt[dst] = NeighborTableEntry(cost);
		nt[dst].mac = dst;
		nt.mac[dst] = dst;

		if (lsdb.count(id))
			lsdb[id].seq++;
		lsdb[id][dst] = cost;
		lsdb.refresh(id);
		rt.calculate();
	} 
	if (op == "rmlink") {
		RouterId dst;
		ss >> dst;

		OUTPUT << "remove neighbor " << dst << std::endl;
		OUTPUT << "remove LSA " << dst << std::endl;
		OUTPUT << "update neighbor state " << dst << " Down" << std::endl;

		std::unique_lock lk(m);

		nt.mac.erase(dst);
		nt.erase(dst);
		nt.timer.erase(dst);
		lsdb.erase(dst);
		lsdb.timer.erase(dst);
		lsdb[id] = nt.toLSA(+1);
		lsdb.refresh(id);
		rt.calculate();
	}
	if (op == "send") {
		RawMessage msg;
		msg.src = id;
		ss >> msg.dst;
		// ss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::getline(ss, msg.payload);

		std::unique_lock lk(m);

		this->sendRaw(msg);
	}
}

void Router::parse(const std::string& content)
{
	DEBUG << "Router::parse called" << std::endl;
	
	std::stringstream ss(content);
	MACAddress src, dst; ss >> src >> dst;
	int type; ss >> type;

	std::unique_lock lk(m);
	if (dst != this->mac || (src != this->mac && !nt.mac.count(src))) {
		DEBUG << "packet dropped, type = " << toString((MessageType)type) << std::endl;
		DEBUG << "srcMac, dstMac = " << src << ", " << dst << std::endl;
		return;
	}
	lk.unlock();

	switch((MessageType)type) {
#define MESSAGE_CASE(msgType, Type, handler) \
	case msgType: { \
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

void Router::handleRaw(const RawMessage& msg)
{
	DEBUG << "Router::handleRaw called" << std::endl;

	if (msg.dst == id) {
		std::osyncstream(std::cout) << "Recv message from " << msg.src << ": " << msg.payload << std::endl;
		return;
	}

	std::unique_lock lk(m);

// Forward	
	if (!rt.count(msg.dst)) {
		INFO << "no route to host" << std::endl;
		return;
	}

	sendRaw(msg);
	std::osyncstream(std::cout) 
		<< "Forward message from " << msg.src << " to " << msg.dst << ": " << msg.payload << std::endl;
}

void Router::handleHello(const HelloMessage& msg)
{
	DEBUG << "Router::handleHello called" << std::endl;
	
	std::unique_lock lk(m);

	if (!nt.count(msg.src)) 
		return;

	LinkState state = nt[msg.src].state;
	if (nt[msg.src].state == LinkState::DOWN) {
		if (msg.received)
			nt[msg.src].state = LinkState::EXCHANGE;
		else
			nt[msg.src].state = LinkState::INIT;
	} else if (nt[msg.src].state == LinkState::INIT) {
		if (msg.received)
			nt[msg.src].state = LinkState::EXCHANGE;
	}
	nt.refresh(msg.src);
	if (nt[msg.src].state != state)
		OUTPUT << "update neighbor state " << msg.src << ' ' << toString(nt[msg.src].state) << std::endl;
}

void Router::handleDBD(const DatabaseDescriptionMessage& msg)
{
	DEBUG << "Router::handleDBD called" << std::endl;
	
	std::unique_lock lk(m);

	if (!nt.count(msg.src)) 
		return;

	std::vector<RouterId> v;
	for (const auto& [i, seq] : msg.dbd) {
		nt[msg.src].dbd[i] = seq;
		if (seq > lsdb[i].seq)
			v.push_back(i);
	}

	if (v.size() == 0) {
		if (nt[msg.src].state != LinkState::FULL) {
			nt[msg.src].state = LinkState::FULL;
			OUTPUT << "update neighbor state " << msg.src << " Full" << std::endl;
			this->sendDBD(msg.src);
		}
	}
	else
		this->sendLSR(msg.src, v);
}

void Router::handleLSR(const LinkStateRequestMessage& msg)
{
	DEBUG << "Router::handleLSR called" << std::endl;

	std::vector<RouterId> v;
	std::copy(msg.begin(), msg.end(), std::back_inserter(v));

	std::unique_lock lk(m);

	this->sendLSU(id, msg.src, v);
}

void Router::handleLSU(const LinkStateUpdateMessage& msg)
{
	DEBUG << "Router::handleLSU called" << std::endl;

	std::unique_lock lk(m);

	std::vector<RouterId> v;
	for (const auto& [i, lsa] : msg) {
		if (!lsdb.count(i) || lsdb[i].seq < lsa.seq) {
			if (!lsdb.count(i))
				OUTPUT << "add LSA " << i << ' ' << lsa.seq << std::endl;
			else
				OUTPUT << "update LSA " << i << ' ' << lsa.seq << std::endl;
			lsdb[i] = lsa;
			lsdb.refresh(i);
			v.push_back(i);
		}
	}

	if (v.size()) {
		rt.calculate();
		this->sendLSU(msg.src, Router::BROADCAST_ID, v);
	}
}

void Router::sendRaw(const RawMessage& msg)
{
	DEBUG << "Router::sendRaw called" << std::endl;

	if (!rt.count(msg.dst)) {
		WARNING << "no route to host" << std::endl;
		return;
	}

	this->send(msg.dst, msg.toString());
}

void Router::sendHello(RouterId dst)
{
	DEBUG << "Router::sendHello called" << std::endl;

	HelloMessage msg(this->id, dst);
	
	if (dst == Router::BROADCAST_ID)
		for (const auto& [i, entry] : nt) {
			msg.dst = i;
			msg.received = (entry.state != LinkState::DOWN);
			this->send(i, msg.toString());
		}
	else {
		msg.received = (nt[dst].state != LinkState::DOWN);
		this->send(dst, msg.toString());
	}
}

void Router::sendDBD(RouterId dst)
{
	DEBUG << "Router::sendDBD called" << std::endl;

	DatabaseDescriptionMessage msg(this->id, dst);
	
	msg.dbd = lsdb.toDatabaseDescription();

	if (dst == Router::BROADCAST_ID)
		for (const auto& [i, entry] : nt)
			if (entry.state == LinkState::EXCHANGE) {
				msg.dst = i;
				this->send(i, msg.toString());
			}
	else
		this->send(dst, msg.toString());
}

void Router::sendLSR(RouterId dst, const std::vector<RouterId>& v)
{
	DEBUG << "Router::sendLSR called" << std::endl;

	LinkStateRequestMessage msg(this->id, dst);
	std::copy(v.begin(), v.end(), std::back_inserter(msg));
	
	this->send(dst, msg.toString());
}

void Router::sendLSU(RouterId src, RouterId dst, const std::vector<RouterId>& v)
{
	DEBUG << "Router::sendLSU called" << std::endl;

	LinkStateUpdateMessage msg(id, dst);

	for (auto i : v)
		msg[i] = lsdb[i];

	if (dst == Router::BROADCAST_ID)
		for (const auto& [i, entry] : nt) 
			if (i != src) {
				msg.dst = i;
				this->send(i, msg.toString());
			}
	else
		this->send(msg.dst, msg.toString());
}
