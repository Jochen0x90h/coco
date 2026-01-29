#include <coco/convert.hpp>
#include <coco/platform/nvic.hpp>
#include <coco/platform/scb.hpp>
#include <coco/platform/adc.hpp>
#include <coco/platform/backup.hpp>
#include <coco/platform/cache.hpp>
#include <coco/platform/can.hpp>
#if defined(STM32G0) || defined(STM32G4)
#include <coco/platform/comp.hpp>
#endif
#if defined(STM32G4) || defined(STM32H5) || defined(STM32U5)
#include <coco/platform/cordic.hpp>
#endif
#include <coco/platform/crc.hpp>
#include <coco/platform/dac.hpp>
#include <coco/platform/exti.hpp>
#include <coco/platform/flash.hpp>
#include <coco/platform/gpio.hpp>
#include <coco/platform/i2c.hpp>
#if defined(STM32G4)
#include <coco/platform/opamp.hpp>
#endif
#include <coco/platform/jump.hpp>
#if defined(STM32F4) || defined(STM32L4) || defined(STM32G4)
#include <coco/platform/qspi.hpp>
#endif
#include <coco/platform/rcc.hpp>
#include <coco/platform/reset.hpp>
#if defined(STM32G0) || defined(STM32F4) || defined(STM32L4)  || defined(STM32G4) || defined(STM32H5) || defined(STM32U3) || defined(STM32U5)
#include <coco/platform/rng.hpp>
#endif
#include <coco/platform/spi.hpp>
#include <coco/platform/timer.hpp>
#include <coco/platform/uart.hpp>
#include <coco/platform/usbd.hpp>
#if defined(STM32G0) || defined(STM32G4)
#include <coco/platform/vref.hpp>
#endif


using namespace coco;

// dummy implementation of debug::sleep
namespace coco {
namespace debug {
void sleep(Microseconds<> time) {}
}
}

enum class Foo {BAR};

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

    //reset(2);

    // adc
    adc::ADC1_INFO.map(dma::DMA1_CH1_INFO); // note that on some processors only some DMA channels work for ADC1, this is noly a compile test
#ifdef HAVE_ADC_DUAL_MODE
    adc::ADC12_INFO.enableClock(adc::ClockConfig());
#endif

    // dma
    dma::DMA1_CH1_INFO.enableClock<dma::Mode::PERIPHERAL_TO_MEMORY>().setCount(50).enable(dma::Config::PRIORITY_HIGH);

    // instruction/data cache
    cache::flush();

    // crc
    crc::enableClock().configure(crc::Config::SIZE_32);

    // qspi
#ifdef HAVE_QUADSPI
    qspi::QUADSPI_INFO.enableClock();
#endif

    // timer
#ifdef TIM1
    timer::TIM1_INFO.irq<timer::Irq::CC>();
    timer::TIM1_INFO.enableClock().start();
#endif

    // uart
    uart::USART1_INFO.mapRx(dma::DMA1_CH1_INFO); // note that on some processors only some DMA channels work for USART1 RX, this is noly a compile test
    uart::USART1_INFO.instance().enable().startRx().startTx().setBaudRate(16MHz, 115200Hz).getBaudRate(16MHz);


    jumpInline(0x8000000);

    return 0;
}
