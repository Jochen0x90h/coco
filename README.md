# CoCo

(Micro)**Co**ntroller **Co**routines

This is the base component of CoCo, a library that enables cooperative multitasking
on bare metal microcontrollers using C++20 coroutines. It removes the need for an
operating system for many use cases. At the same time, it can be used as a programming
paradigm on classical operating systems (Windows, Linux, MacOS).

## Import
Add coco/\<version> to your conanfile where version corresponds to the git tags

## Features
* Coroutines for asynchronous operation
* Unified programming interface for desktop OS (Windows/MacOS/Linux) and microcontrollers
* Separate coco libraries for "drivers" such as coco-spi, coco-i2c, coco-usb etc.
* Helper classes for time, frequency, queues, strings and vectors of 2-4 components

## Libraries

### Base Libraries

Library        | Purpose
---------------|-----------
coco-toolchain | Target specific CMake toolchain files for configuring MCU, FPU etc.
coco           | Base library containing utility classes and MCU register definitions
coco-devboards | Collection of development boards
coco-loop      | Event loop (Windows and MCU)
coco-device    | Device abstractions

### Input/Output

Library        | Purpose
---------------|-----------
coco-io        | Virtual GPIO port library (only MCU)

### Connectivity

Library        | Purpose
---------------|-----------
coco-i2c       | I2C/two wire interface library (MCU)
coco-spi       | SPI library (MCU)
coco-uart      | UART library (Windows and MCU)
coco-udp       | UDP/IP library (Windows)
coco-tcp       | TCP/IP library (Windows)
coco-usb       | USB library for host (Windows and MCU) and device (MCU)

### Memory

Library        | Purpose
---------------|-----------
coco-flash     | Support for MCU flash and flash emulation
coco-storage   | Flash storage with wear leveling

### File

Library        | Purpose
---------------|-----------
coco-file      | File access library (only Windows)
coco-pcap      | Simple library for storing Wireshark pcap files

### Other

Library        | Purpose
---------------|-----------
coco-font      | Simple pixel font library, also used by the emulator
coco-led-strip | LED strp driver for WS2812 etc.
coco-random    | Hardware random number generator

## Supported Platforms
* OS
    * Windows
    * Todo: macOS
    * Todo: Linux
* nRF52 bare metal
    * nRF52840
* STM32 bare metal
    * STM32F0
    * STM32F3
    * STM32F4
    * STM32C0
    * STM32G4

Pass -o platform=\<platform> to conan to set the platform. When no platform is given, the platform dependent code is omitted in the resulting conan package.

## Development

### General

Install conan 2.x and the arm gcc toolchain

Use coco projects for the first time: Follow instructions in [support/conan/README.md](support/conan/README.md)

Install coco projects to local conan repository
- Go into coco-toolchain and run python create.py
- Go into coco and run python create.py
- Go into coco-devboards run python create.py
- Go into coco-font run python create.py
- Go into coco-loop run python create.py
- Repeat for all other coco projects


### Visual Studio Code

Install these extensions
- C/C++
- C/C++ Extension Pack

Add to settings.json
```json
"cmake.useCMakePresets": "always",
"cmake.debugConfig": {
    "cwd": "${workspaceFolder}"
}
```

Use in Visual Studio Code
- Open a project folder (e.g. coco-loop)
- Install conan dependencies by running configure.py (right click and "Run Python File in Terminal" to generate CMakeUserPresets.json)
- Sometimes it is necessary to close VSCode and open it again
- Select CMake Configure preset and Build preset (e.g. stm32c031xx)
- Select CMake Build target (e.g. LoopTest-stm32c031nucleo)
- Build (gear icon in the status bar)
- Now debug (icon: bug) or run (icon: play) the application
- If a dependency or the conanfile.py has changed (e.g. new version), run configure.py again
