#include "system.hpp"


#define xstr(s) str(s)
#define str(s) #s

namespace coco {
namespace system {

StringBuffer<16> name() {
    return xstr(DEVICE);
}

} // namespace system
} // namespace coco
