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
