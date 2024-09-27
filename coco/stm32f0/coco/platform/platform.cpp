#include "platform.hpp"


extern "C" {

// implement atomic exchange using global disable/enable interrupts
unsigned int __atomic_exchange_4(volatile void *mem, unsigned int val, int model) {
	__disable_irq();
	int32_t r = *reinterpret_cast<volatile unsigned int *>(mem);
	*reinterpret_cast<volatile unsigned int *>(mem) = val;
	__enable_irq();
	return r;
}

}
