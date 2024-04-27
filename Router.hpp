#pragma once

#include "LinkStateDatabase.hpp"
#include "RoutingTable.hpp"
#include "NeighborTable.hpp"
#include "RawMessage.hpp"
#include "HelloMessage.hpp"
#include "DatabaseDescriptionMessage.hpp"
#include "LinkStateRequestMessage.hpp"
#include "LinkStateUpdateMessage.hpp"
#include "HelloTimer.hpp"
#include "LSATimer.hpp"

#include <thread>
#include <mutex>

using RouterId = int;

struct Router {
	RouterId id;
	LinkStateDatabase lsdb;
	RoutingTable rt;
	NeighborTable nt;

	std::jthread listener;
	
	HelloTimer helloTimer;
	LSATimer lsaTimer;

	static const RouterId BROADCAST_ID = 65535;
	static const size_t RECV_BUFSIZE = 1024 * 1024;

	Router(RouterId id);
	
	void listen(std::string host, int port);
	void send(RouterId id, std::string content);
	void command(std::string input);

	void parse(std::string input);
	void handleRaw(RawMessage);
	void handleHello(HelloMessage);
	void handleDBD(DatabaseDescriptionMessage);
	void handleLSR(LinkStateRequestMessage);
	void handleLSU(LinkStateUpdateMessage);

	void sendRaw(RouterId);
	void sendHello(RouterId);
	void sendDBD(RouterId);
	void sendLSR(RouterId);
	void sendLSU(RouterId);
};
