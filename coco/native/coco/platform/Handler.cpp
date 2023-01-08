#include "Handler.hpp"


namespace coco {

YieldHandler::~YieldHandler() {
}
YieldHandlerList yieldHandlers;

TimeHandler::~TimeHandler() {
}
TimeHandlerList timeHandlers;

SocketHandler::~SocketHandler() {
}
SocketHandlerList socketHandlers;

} // namespace coco
