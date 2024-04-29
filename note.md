## Threads

- command
    - [x] addlink / setlink
    - [x] rmlink
    - [x] send
- listen udp
    - [x] raw
    - [x] hello
    - [x] dbd
    - [x] lsr
    - [x] lsu
- timer
    - [x] hello
    - [x] dbd
    - [x] lsa
    - [x] lsdb
    - [x] router down

mutex on lsdb, rt, nt
lock only in these function to prevent deadlock

## Communication formats

- Message
    - RouterId src, dst
    - virtual std::string toString()

- Hello: Message
    - received

- DatabaseDescription: Message, std::map<RouterId, Sequence>

- LinkStateRequest: Message, std::vector<RouterId>

- LinkStateUpdate: Message, std::map<int, LinkStateAdvertisement>

## Components

- LinkState: enum {DOWN, INIT, EXCHANGE, FULL}

- LinkStateAdvertisement: map<RouterId, Cost>
    - Sequence

- LinkStateDatabase: std::map<RouterId, LinkStateAdvertisement>
    - std::map<RouterId, std::jthread>
    - DatabaseDescription toDatabaseDescription()
    - void update(LinkStateUpdate)

- RoutingTableEntry
    - Cost
    - NextHop

- RoutingTable: std::map<RouterId, RoutingTableEntry>
    - void calculate()

- NeighborTableEntry
    - Cost
    - State
    - DatabaseDescription

- NeighborTable: std::map<RouterId, NeighborTableEntry>


## Timer

- Timer
    - std::function hypervisor_func
    - std::fucntion runner_func
    - std::jthread hypervisor
    - std::jthread runner

    - void refresh(void)

- HelloTimer: Timer
    - Router& router
 
- LSATimer: Timer
    - Router& router

- DBDTimer: Timer
    - Router& router

## Main

- Router
    - LinkStateDatabase lsdb
    - RoutingTable rt
    - NeighborTable nt

    - std::jthread listener
    
    - HelloTimer
    - DBDTimer
    - LSATimer

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

