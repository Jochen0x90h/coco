# Conan

If you use conan for the first time, run

```console
$ conan profile detect
```

It creates a default profile in ~/.conan2/profiles

## Windows
The default profile should contain these lines:

```
compiler=msvc
compiler.cppstd=20
```

If not, Install Visual Studio Community Edition (in addition to VSCode!)
with C++ for desktop option and/or change cppstd to 20.

## Debug Profile

Create debug profile ~/.conan2/profiles/debug by copying the default profile and set

```
build_type=Debug
```

## Arm Compiler

Copy the other profiles (armgcc, armv6, armv7) from the subdirectory for your platform directory into ~/.conan2/profiles

armgcc file: Adjust compiler.version and tools.build:compiler_executable (both for gcc and g++) if necessary


## Presets

Copy cpresets.txt from the subdirectory for your platform into the root of the project and modify it to contain all
desired platforms (native / microcontrollers).

Then run

```console
$ python cinstall.py
```

It generates CMakeUserPresets.json which can be used by IDEs such as VSCode

Copy cpresets.txt also to the root of all other coco libraries and projects using coco (distribute.py does it for
"neighboring" coco projects starting with "coco-")


## Install to Conan Cache

To install to the local conan cache, run

```console
$ python ccreate.py
```

Now a coco library can be used by other projects as dependency by adding it to the conanfile.
