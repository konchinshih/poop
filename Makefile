CXX ?= g++

COMPILE_FLAGS += -std=c++20 #-g -fsanitize=address,undefined
LINK_FLAGS += #-g -fsanitize=address,undefined

.PHONY: all clean

all: ospf

ospf: main.o misc.o Router.o \
	DatabaseDescription.o LinkStateDatabase.o \
	LinkStateAdvertisement.o \
	RoutingTable.o \
	NeighborTable.o \
	LinkStateAdvertisement.o \
	Message.o \
	RawMessage.o \
	HelloMessage.o \
	LinkStateRequestMessage.o \
	LinkStateUpdateMessage.o \
	DatabaseDescriptionMessage.o \
	Timer.o \
	HelloTimer.o \
	LSATimer.o
	
	$(CXX) -o $@ $^ $(LINK_FLAGS)

%.o: %.cpp
	$(CXX) $< -c -o $@ $(COMPILE_FLAGS)

clean: 
	- rm -f ospf *.o
