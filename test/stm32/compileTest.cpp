#include <coco/platform/nvic.hpp>
#include <coco/platform/adc.hpp>
#include <coco/platform/dac.hpp>
#include <coco/platform/exti.hpp>
#include <coco/platform/gpio.hpp>
#include <coco/platform/i2c.hpp>
#include <coco/platform/spi.hpp>
#include <coco/platform/timer.hpp>
#include <coco/platform/usart.hpp>


int main() {
    // only test if the included headers compile

    int x = coco::nvic::guarded(0, []() {return 10;});

    return x;
}
