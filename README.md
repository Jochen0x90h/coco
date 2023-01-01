# CoCo

(Micro)**Co**ntroller **Co**routines

This is the base component of CoCo, a library that enables cooperative multitasking
on bare metal microcontrollers using C++20 coroutines. It removes the need for an
operating system for many use cases.

## Import
Add coco/<version> to your conanfile where version corresponds to the git tags

## Features
* Central event loop
* Time with millisecond resolution
* Suspending of a coroutine for a given duration or until a given time with co_await sleep(...)
* Separate libraries for "drivers" such as SPI, USB etc.
* Also runs on Windows/MacOS/Linux using poll()

## Supported Platforms
* Native
  * native
* nRF52
  * nrf52805
  * nrf52810
  * nrf52811
  * nrf52820
  * nrf52833
  * nrf52840
* STM32F0
  * stm32f030x6
  * stm32f038xx
  * stm32f058xx
  * stm32f072xb
  * stm32f030x8
  * stm32f042x6
  * stm32f070x6
  * stm32f078xx
  * stm32f030xc
  * stm32f048xx
  * stm32f070xb
  * stm32f091xc
  * stm32f031x6
  * stm32f051x8
  * stm32f071xb
  * stm32f098xx

Pass -o platform=\<platform> to conan to set the platform. When no platform is given, the platform dependent code is omitted in the resulting conan package.
