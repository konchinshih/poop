## Components

- DatabaseDescription: std::map<RouterId, Sequence>

- LinkStateDatabase: std::map<RouterId, LinkStateAdvertisement>
    - std::map<RouterId, std::jthread>
    - DatabaseDescription toDatabaseDescription()
    - void update(LinkStateUpdate)


- RoutingTable: std::map<RouterId, RoutingTableEntry>
    - void calculate()

- RoutingTableEntry
    - Cost
    - NextHop

- NeighborTable: std::map<RouterId, NeighborTableEntry>

- NeighborTableEntry
    - Cost
    - State
    - DatabaseDescription

- State: enum {DOWN, INIT, EXCHANGE, FULL}

## Timer

- Timer: std::jthread

- TimeQueue
    - void deploy(Router&)

- HelloTimeQueue: TimeQueue, std::priority_queue<int>
 
- SelfLSARefreshTimeQueue: Timequeue, std::priority_queue<int>

## Messages

- Message
    - virtual std::string toString()

- Hello: Message

- LinkStateAdvertisement: Message, map<RouterId, Cost>
    - Sequence

- LinkStateRequest: Message, vector<RouterId>

- LinkStateUpdate: Message, map<int, LinkStateAdvertisement>

## Main

- Router
    - LinkStateDatabase lsdb
    - RoutingTable rt
    - NeighborTable nt

    - std::jthread listener
    - std::jthread timer
    - std::mutex io

    - void listen(Port)
    - void send(RouterId, string)

    - void handleHello(Hello)
    - void handleLSA(LinkStateAdvertisement)
    - void handleLSR(LinkStateRequest)
    - void handleLSU(LinkStateUpdate)

    - void sendHello(RouterId)
    - void sendLSA(RouterId)
    - void sendLSR(RouterId)
    - void sendLSU(RouterId)

## Format

### General

```
[type]\n
[src] [dst]\n
[payload]
```

### Raw

```
0\n
[src] [dst]\n
[message]
```

### Hello

```
1\n
[src] [dst]\n
[received]
```

### 

