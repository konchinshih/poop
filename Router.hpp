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
#include "DBDTimer.hpp"
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
	DBDTimer dbdTimer;
	LSATimer lsaTimer;

	std::mutex sendLock;

	static const RouterId BROADCAST_ID = 65535;
	static const size_t RECV_BUFSIZE = 1024 * 1024;

	Router(RouterId id);
	
	void listen(const std::string& host, int port);
	void send(RouterId id, const std::string& content);
	void command(const std::string& input);

	void parse(const std::string& input);
	void handleRaw(const RawMessage&);
	void handleHello(const HelloMessage&);
	void handleDBD(const DatabaseDescriptionMessage&);
	void handleLSR(const LinkStateRequestMessage&);
	void handleLSU(const LinkStateUpdateMessage&);

	// send or forward
	void sendRaw(const RawMessage&);
	
	// send
	void sendHello(RouterId dst);

	// send or flood all
	void sendDBD(RouterId dst);

	// send based on dbd message
	void sendLSR(RouterId dst, const std::vector<RouterId>&);

	// send or flood except source
	void sendLSU(RouterId src, RouterId dst, const std::vector<RouterId>&);
};
