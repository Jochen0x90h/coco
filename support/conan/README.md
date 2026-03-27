# Conan

If you use conan for the first time, run

```console
$ conan profile detect
```

It creates a default profile in ~/.conan2/profiles

## Windows
Open default profile (~/.conan2/profiles/default) with text editor and make sure
it contains these lines:

```
compiler=msvc
compiler.cppstd=23
```

If the compiler is missing, install Visual Studio Community Edition (in addition to VSCode!) with C++ for desktop option and run conan profile detect again.

Change compiler.cppstd to 23.

## Linux
Open default profile (~/.conan2/profiles/default) with text editor and make sure
it contains these lines:

```
compiler=gcc
compiler.cppstd=23
```

Change compiler.cppstd to 23.

If you want to let conan install missing packages, add these lines at the end of the profile:

```
[conf]
tools.system.package_manager:mode=install
tools.system.package_manager:sudo=True
```

## Debug Profile

Create debug profile ~/.conan2/profiles/debug by copying the default profile and set

```
build_type=Debug
```

## Arm Compiler

Copy the profiles (armgcc, armv6, armv7) from the profiles subdirectory into ~/.conan2/profiles

Open armgcc profile (~/.conan2/profiles/armgcc) with text editor and adjust compiler.version if necessary.
Obtain compiler version:

```
arm-none-eabi-gcc --version
```

If the compiler is not found, add the full path to tools.build:compiler_executable (both for gcc and g++).

## Presets

Copy cpresets.txt into the root of the project and modify it to contain all desired platforms (native / microcontrollers).

Copy cpresets.txt also to the root of all other coco libraries and projects using coco
(distribute.py does it for "neighboring" coco projects starting with "coco-").

Now continue reading the main README.md
