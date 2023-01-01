#pragma once

#include <coco/LinkedList.hpp>
#include <coco/Time.hpp>
#include <cstdint>
#ifdef _WIN32
using Socket = uint64_t;
#else
using Socket = int;
#endif


namespace coco {

// list of file descriptors to observe readable/writable events (used in Network.cpp)
class SocketHandler : public LinkedListNode {
public:
	virtual ~SocketHandler();
	virtual void activate(uint16_t events) = 0;

	Socket socket = -1;
	short int events;
};
using SocketHandlerList = LinkedList<SocketHandler>;
extern SocketHandlerList socketHandlers;

// timeouts for Timer and Calendar
class TimeHandler : public LinkedListNode {
public:
	virtual ~TimeHandler();
	virtual void activate() = 0;

	Time time;
};
using TimeHandlerList = LinkedList<TimeHandler>;
extern TimeHandlerList timeHandlers;

} // namespace coco
