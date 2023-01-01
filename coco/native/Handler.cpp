#include "Handler.hpp"


namespace coco {

SocketHandler::~SocketHandler() {
}
SocketHandlerList socketHandlers;

TimeHandler::~TimeHandler() {
}
TimeHandlerList timeHandlers;

} // namespace coco
