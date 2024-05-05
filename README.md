# CPUInfo
A C++ Header Only Include that Wraps either the CPUInfo syscall, or a emulated windows CPUInfo syscall(In Linux)

## Purpose
Add a easy interface to allow programs access to every piece of information on the processor, even with those tricky hybrid CPUs

## Including
Either drop this into an includes folder, or wrap in your own folder inside of an includes folder, and use:
```cpp
#include "cpuinfo.h"
//or  if wrapped in a folder
#include "{folder_name}/cpuinfo.h"
```
and the preprocessors should handle the rest, abstracting over the archetechure so your program does not need to explicitly change calls based on archetechure

## Use
to use the cpuinfo library, use the `CPU` type described in `cpuinfo.h`, this on creation will spawn threads at runtime on every thread to get the capabilities of each hardware thread, and will also know whether or not the cpu is a hybrid. as of yet I have not added a explicit way to determine which cores are the powerfull cores, but rather allow code running to quickly index what the capabilities are of the thread its running on by hardware thread ID

### Do I need to synchronize anything?
No, the creation of the CPU obj is expensive but caches the results for later use, this ensures that these rather expensive calls occur once, and are completed from there, the CPU object has a internal register that will allow it to busy-wait until all threads have completed the cpuid task, then allow the calling thread to continue without the developer ever having to tough semaphores or mutexes

### why tho?
This exists to allow programs to create a faster path that may not be supported by the majority of hardware yet, without breaking support for said hardware, allowing a program to either choose an equivalent path to softly ignore hardware incompatability, or allowing a program a low-effort way to error and crash, telling the user why
