//this file is only ever included by default when compiling for ARM, or by direct inclusion which will cause issues
namespace CPUInfoShifts {
    //x86 extensions exist to prevent the need of preprocessors in your code, they will not return sensible values though, please either use shifts0::HELIUM or shifts0::NEON as these values work
    enum shifts0 {
        _3DNOW, _3DNOWEXT, ABM, ADX, AES, AVX, AVX2, AVX512CD, AVX512ER, AVX512F, AVX512PF, BMI1, BMI2, CLFSH, CMPXCHG16B, CX8, ERMS, F16C, FMA, FSGSBASE, FXSR, HLE, INVPCID, LAHF, LZCNT, MMX, MMXEXT, MONITOR, MOVBE, MSR, OSXSAVE, PCLMULQDQ, NEON = 0
    };
    //x86 extensions exist to prevent the need of preprocessors in your code, they will not return sensible values though, please either use shifts0::HELIUM or shifts0::NEON as these values work
    enum shifts1 {
        POPCNT, PREFETCHWT1, RDRAND, RDSEED, RDTSCP, RTM, SEP, SHA, SSE, SSE2, SSE3, SSE4_1, SSE4_2, SSE4a, SSSE3, SYSCALL, TBM, XOP, XSAVE
    };
}
#pragma once

#include <cassert>
#include <cstdio>
#include <cstring>

#include <elf.h>
#include <fcntl.h>
#include <linux/auxvec.h>
#include <unistd.h>
//this file is nonsense on x86, I have to trust the internet on this one since I do not have an ARM computer to test on, besides, macs will not work out of the factory with vulkan anyways #ThanksApple
//todo: make arm work outside linux
void initFlags(ProcessorDescriptor desc)
{
    //defining this by arch allows you to use if statements instead of preprocessors
    desc.ar = "ARM";
    desc.brandname = "Generic ARM Processor";
    desc.vendor = "Unknown";
#if defined(__aarch64__)
    desc.flags = 0 | (true << CPUInfoShifts::shifts0::NEON);
    desc.ar = "ARM64";
#else

    auto cpufile = open("/proc/self/auxv", O_RDONLY);
    assert(cpufile);

    Elf32_auxv_t auxv;

    if (cpufile >= 0)
    {
        const auto size_auxv_t = sizeof(Elf32_auxv_t);
        desc.flags = 0;
        while (read(cpufile, &auxv, size_auxv_t) == size_auxv_t)
        {
            if (auxv.a_type == AT_HWCAP)
            {
                bool hasNeon = (auxv.a_un.a_val & 4096) != 0;
                desc.flags = desc.flags | (hasNeon << CPUInfoShifts::shifts0::NEON);
                break;
            }
        }

        close(cpufile);
    }
    else
    {
        info.m_has_neon = false;
    }
#endif
}