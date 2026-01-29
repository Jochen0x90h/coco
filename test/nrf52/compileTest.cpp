#include <coco/convert.hpp>
#include <coco/platform/nvic.hpp>
#include <coco/platform/cache.hpp>
#include <coco/platform/gpio.hpp>
#include <coco/platform/ppi.hpp>
#include <coco/platform/rng.hpp>
#include <coco/platform/rtc.hpp>
#include <coco/platform/spim.hpp>
#include <coco/platform/timer.hpp>
#include <coco/platform/uart.hpp>


using namespace coco;

enum class Foo {BAR};

volatile uint8_t data[5];
volatile const uint8_t constData[5] = {1, 2, 3, 4, 5};

int main() {
    // only test if the code compiles

    // convert
    {
        int x = 10;
        int64_t y = 10;
        std::atomic<int> a = 50;
        dec(x);
        dec(y);
        dec(Foo::BAR);
        dec(a);
        hex(x);
        hex(y);
        hex(Foo::BAR);
        hex(a);
    }

    // nvic
    {
        __attribute__((unused)) int result = nvic::guarded(0, []() {
            // this is executed with interrupt 0 disabled
            return 10;
        });

    }

    // instruction/data cache
    cache::flush();

    // spi master
    spim::SPIM0_INFO.instance()
        .configure(spim::Format::DEFAULT)
        .enable()
        .setRxData(constData, 0)
        .setTxData(data, 0);

    // uart
    uart::UART0_INFO.instance()
        .enable(uart::Config::DEFAULT, uart::Format::DEFAULT, 38400kHz)
        .setBaudRate(115200Hz);
    uart::UARTE0_INFO.instance()
        .enable(uart::Config::DEFAULT, uart::Format::DEFAULT, 38400kHz)
        .setBaudRate(115200Hz)
        .setRxData(constData, 0)
        .setTxData(data, 0);
}
