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

/**
 * Yield handler for loop::yield() and emulated transfers
 */
class YieldHandler : public LinkedListNode {
public:
	virtual ~YieldHandler();
	virtual void activate() = 0;
};
using YieldHandlerList = LinkedList<YieldHandler>;
extern YieldHandlerList yieldHandlers;

/**
 * Time handler for loop::sleep() and Calendar
 */
class TimeHandler : public LinkedListNode {
public:
	virtual ~TimeHandler();
	virtual void activate() = 0;

	Time time;
};
using TimeHandlerList = LinkedList<TimeHandler>;
extern TimeHandlerList timeHandlers;

/**
 * Socket handlers to observe readable/writable events e.g. for networking
 */
class SocketHandler : public LinkedListNode {
public:
	virtual ~SocketHandler();
	virtual void activate(uint16_t events) = 0;

	Socket socket = -1;
	short int events;
};
using SocketHandlerList = LinkedList<SocketHandler>;
extern SocketHandlerList socketHandlers;

} // namespace coco
