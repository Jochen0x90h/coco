# CoCo

(Micro)**Co**ntroller **Co**routines

This is the base component of CoCo, a library that enables cooperative multitasking
on bare metal microcontrollers using C++20 coroutines. It removes the need for an
operating system for many use cases.

## Import
Add coco/\<version> to your conanfile where version corresponds to the git tags

## Features
* Central event loop
* Time with millisecond resolution
* Suspending of a coroutine for a given duration or until a given time with co_await sleep(...)
* Separate libraries for "drivers" such as SPI, USB etc.
* Also runs on Windows/MacOS/Linux using poll()

## Supported Platforms
* Native
  * Windows
  * Todo: macOS
  * Todo: Linux
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
* STM32G4
  * stm32g431xx
  * stm32g471xx
  * stm32g474xx
  * stm32g484xx
  * stm32g4a1xx
  * stm32g441xx
  * stm32g473xx
  * stm32g483xx
  * stm32g491xx

Pass -o platform=\<platform> to conan to set the platform. When no platform is given, the platform dependent code is omitted in the resulting conan package.

## Development

### Visual Studio Code

Install these extensions
- C/C++
- C/C++ Extension Pack
- conan (disroop)

Add to settings.json
```
"cmake.useCMakePresets": "always",
"cmake.debugConfig": {
    "cwd": "${workspaceFolder}"
}
```

Install the arm gcc toolchain

Open coco* projects for the first time
- Copy the conan profiles from coco/support/conan/ to your ~/.conan/profiles (make sure CC and CXX point to the arm toolchain)
- Copy conan-settings.json from coco/support/vscode to .vscode/ in each project
- Copy presets.py from coco/support/vscode to top level directory of each project
- Open Visual Studio Code
- Install conan dependencies by clicking on "Install" on the blue bar (cloud download icon)
- Run presets.py by right-clicking and selecting "Run Python File in Terminal" to generate CMakeUserPresets.json

Configure and run
- If "requires" in conanfile.py have changed (e.g. new version), click on "Install" again
- Select CMake configure and build presets on the blue bar (tools icon)
- Build (gear icon)
- Now debug (bug icon) or run (play icon) the application
