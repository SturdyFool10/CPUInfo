
# CPUInfo
A C++ header-only library that provides an interface to system CPU information, leveraging either the native CPUInfo syscall or an emulated Windows CPUInfo syscall on Linux.

## Purpose
The purpose of CPUInfo is to provide an easy interface that allows applications to access comprehensive details about the processor, including data on complex hybrid CPUs.

## Installation
Simply place the header file in your project's include directory, or within a dedicated subdirectory in the includes folder. You can include it in your project with:
```cpp
#include "cpuinfo.h"
// If in a subdirectory:
#include "{folder_name}/cpuinfo.h"
```
The preprocessor directives included in the library handle architecture abstraction, eliminating the need for architecture-specific calls in your application.

## Usage
To utilize the CPUInfo library, create an instance of the `CPU` class defined in `cpuinfo.h`. This class initializes threads at runtime for each hardware thread to ascertain their capabilities and identifies whether the CPU is hybrid. Currently, there is no direct method to distinguish between high-performance and efficiency cores; however, the library facilitates quick access to thread capabilities via hardware thread ID.

### Thread Safety
The initialization of the `CPU` object is resource-intensive but is designed to cache the results for subsequent use. This design ensures that costly operations are performed only once. The object uses an internal mechanism to wait until all threads have finished executing the CPUID tasks before proceeding, obviating the need for external synchronization tools like semaphores or mutexes.

### Rationale
This library was developed to enable applications to exploit advanced hardware capabilities that may not be widely supported yet, without compromising backward compatibility. It provides a straightforward approach for applications to either adapt to or gracefully handle hardware limitations, or to allow the developer to create a message stating that hardware must support a feature for the program to run
