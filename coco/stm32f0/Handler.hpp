#pragma once

#include <coco/LinkedList.hpp>


namespace coco {

/**
 * Event handler that handles activity of the peripherals
 */
class Handler : public LinkedListNode {
public:
	virtual ~Handler();
	virtual void handle() = 0;
};
using HandlerList = LinkedList<Handler>;
extern HandlerList handlers;

} // namespace coco
