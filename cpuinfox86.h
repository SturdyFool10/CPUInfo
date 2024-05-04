//this file is only included when compiling for x86 or x86-64, manual forced inclusion is not recommended
#pragma once
#include <iostream>
#include <vector>
#include <bitset>
#include <array>

//this file will be nonsense to arm processors, there will be errors, but it is normal but will not cause you trouble as this file isn't included anywhere in ARM compilaiton, just move along
namespace CPUInfoShifts {
    //ARM extensions exist to prevent the need for preprocessors in your code, just so it exists, it will return non-corelated data but it won't error
    enum shifts0 {
        _3DNOW, _3DNOWEXT, ABM, ADX, AES, AVX, AVX2, AVX512CD, AVX512ER, AVX512F, AVX512PF, BMI1, BMI2, CLFSH, CMPXCHG16B, CX8, ERMS, F16C, FMA, FSGSBASE, FXSR, HLE, INVPCID, LAHF, LZCNT, MMX, MMXEXT, MONITOR, MOVBE, MSR, OSXSAVE, PCLMULQDQ, NEON = 0
    };
    enum shifts1 {
        POPCNT, PREFETCHWT1, RDRAND, RDSEED, RDTSCP, RTM, SEP, SHA, SSE, SSE2, SSE3, SSE4_1, SSE4_2, SSE4a, SSSE3, SYSCALL, TBM, XOP, XSAVE
    };
}

#ifdef _WIN32
    #include <intrin.h>
#else
    #include <stdint.h>
void __cpuid(int CPUInfo[4], int InfoType)
{
    __asm {
        mov esi, CPUInfo
        mov eax, InfoType
        xor ecx, ecx
        cpuid
        mov dword ptr[esi + 0], eax
        mov dword ptr[esi + 4], ebx
        mov dword ptr[esi + 8], ecx
        mov dword ptr[esi + 12], edx
    }
}
void __cpuidex(int CPUInfo[4], int InfoType, int ECXValue)
{
    __asm
    {
        mov esi, CPUInfo
        mov eax, InfoType
        mov ecx, ECXValue
        cpuid
        mov dword ptr[esi + 0], eax
        mov dword ptr[esi + 4], ebx
        mov dword ptr[esi + 8], ecx
        mov dword ptr[esi + 12], edx
    }
}
#endif

class InstructionSet
{
    // forward declarations
    class InstructionSet_Internal;

public:
    // getters
    static std::string Vendor(void) { return CPU_Rep.vendor_; }
    static std::string Brand(void) { return CPU_Rep.brand_; }

    static bool SSE3(void) { return CPU_Rep.f_1_ECX_[0]; }
    static bool PCLMULQDQ(void) { return CPU_Rep.f_1_ECX_[1]; }
    static bool MONITOR(void) { return CPU_Rep.f_1_ECX_[3]; }
    static bool SSSE3(void) { return CPU_Rep.f_1_ECX_[9]; }
    static bool FMA(void) { return CPU_Rep.f_1_ECX_[12]; }
    static bool CMPXCHG16B(void) { return CPU_Rep.f_1_ECX_[13]; }
    static bool SSE41(void) { return CPU_Rep.f_1_ECX_[19]; }
    static bool SSE42(void) { return CPU_Rep.f_1_ECX_[20]; }
    static bool MOVBE(void) { return CPU_Rep.f_1_ECX_[22]; }
    static bool POPCNT(void) { return CPU_Rep.f_1_ECX_[23]; }
    static bool AES(void) { return CPU_Rep.f_1_ECX_[25]; }
    static bool XSAVE(void) { return CPU_Rep.f_1_ECX_[26]; }
    static bool OSXSAVE(void) { return CPU_Rep.f_1_ECX_[27]; }
    static bool AVX(void) { return CPU_Rep.f_1_ECX_[28]; }
    static bool F16C(void) { return CPU_Rep.f_1_ECX_[29]; }
    static bool RDRAND(void) { return CPU_Rep.f_1_ECX_[30]; }

    static bool MSR(void) { return CPU_Rep.f_1_EDX_[5]; }
    static bool CX8(void) { return CPU_Rep.f_1_EDX_[8]; }
    static bool SEP(void) { return CPU_Rep.f_1_EDX_[11]; }
    static bool CMOV(void) { return CPU_Rep.f_1_EDX_[15]; }
    static bool CLFSH(void) { return CPU_Rep.f_1_EDX_[19]; }
    static bool MMX(void) { return CPU_Rep.f_1_EDX_[23]; }
    static bool FXSR(void) { return CPU_Rep.f_1_EDX_[24]; }
    static bool SSE(void) { return CPU_Rep.f_1_EDX_[25]; }
    static bool SSE2(void) { return CPU_Rep.f_1_EDX_[26]; }

    static bool FSGSBASE(void) { return CPU_Rep.f_7_EBX_[0]; }
    static bool BMI1(void) { return CPU_Rep.f_7_EBX_[3]; }
    static bool HLE(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[4]; }
    static bool AVX2(void) { return CPU_Rep.f_7_EBX_[5]; }
    static bool BMI2(void) { return CPU_Rep.f_7_EBX_[8]; }
    static bool ERMS(void) { return CPU_Rep.f_7_EBX_[9]; }
    static bool INVPCID(void) { return CPU_Rep.f_7_EBX_[10]; }
    static bool RTM(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[11]; }
    static bool AVX512F(void) { return CPU_Rep.f_7_EBX_[16]; }
    static bool RDSEED(void) { return CPU_Rep.f_7_EBX_[18]; }
    static bool ADX(void) { return CPU_Rep.f_7_EBX_[19]; }
    static bool AVX512PF(void) { return CPU_Rep.f_7_EBX_[26]; }
    static bool AVX512ER(void) { return CPU_Rep.f_7_EBX_[27]; }
    static bool AVX512CD(void) { return CPU_Rep.f_7_EBX_[28]; }
    static bool SHA(void) { return CPU_Rep.f_7_EBX_[29]; }

    static bool PREFETCHWT1(void) { return CPU_Rep.f_7_ECX_[0]; }

    static bool LAHF(void) { return CPU_Rep.f_81_ECX_[0]; }
    static bool LZCNT(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_ECX_[5]; }
    static bool ABM(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[5]; }
    static bool SSE4a(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[6]; }
    static bool XOP(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[11]; }
    static bool TBM(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[21]; }

    static bool SYSCALL(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[11]; }
    static bool MMXEXT(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[22]; }
    static bool RDTSCP(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[27]; }
    static bool _3DNOWEXT(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[30]; }
    static bool _3DNOW(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[31]; }

private:
    static const InstructionSet_Internal CPU_Rep;

    class InstructionSet_Internal
    {
    public:
        InstructionSet_Internal()
            : nIds_{ 0 },
            nExIds_{ 0 },
            isIntel_{ false },
            isAMD_{ false },
            f_1_ECX_{ 0 },
            f_1_EDX_{ 0 },
            f_7_EBX_{ 0 },
            f_7_ECX_{ 0 },
            f_81_ECX_{ 0 },
            f_81_EDX_{ 0 },
            data_{},
            extdata_{}
        {
            //int cpuInfo[4] = {-1};
            std::array<int, 4> cpui;

            // Calling __cpuid with 0x0 as the function_id argument
            // gets the number of the highest valid function ID.
            __cpuid(cpui.data(), 0);
            nIds_ = cpui[0];

            for (int i = 0; i <= nIds_; ++i)
            {
                __cpuidex(cpui.data(), i, 0);
                data_.push_back(cpui);
            }

            // Capture vendor string
            char vendor[0x20];
            memset(vendor, 0, sizeof(vendor));
            *reinterpret_cast<int*>(vendor) = data_[0][1];
            *reinterpret_cast<int*>(vendor + 4) = data_[0][3];
            *reinterpret_cast<int*>(vendor + 8) = data_[0][2];
            vendor_ = vendor;
            if (vendor_ == "GenuineIntel")
            {
                isIntel_ = true;
            }
            else if (vendor_ == "AuthenticAMD")
            {
                isAMD_ = true;
            }

            // load bitset with flags for function 0x00000001
            if (nIds_ >= 1)
            {
                f_1_ECX_ = data_[1][2];
                f_1_EDX_ = data_[1][3];
            }

            // load bitset with flags for function 0x00000007
            if (nIds_ >= 7)
            {
                f_7_EBX_ = data_[7][1];
                f_7_ECX_ = data_[7][2];
            }

            // Calling __cpuid with 0x80000000 as the function_id argument
            // gets the number of the highest valid extended ID.
            __cpuid(cpui.data(), 0x80000000);
            nExIds_ = cpui[0];

            char brand[0x40];
            memset(brand, 0, sizeof(brand));

            for (int i = 0x80000000; i <= nExIds_; ++i)
            {
                __cpuidex(cpui.data(), i, 0);
                extdata_.push_back(cpui);
            }

            // load bitset with flags for function 0x80000001
            if (nExIds_ >= 0x80000001)
            {
                f_81_ECX_ = extdata_[1][2];
                f_81_EDX_ = extdata_[1][3];
            }

            // Interpret CPU brand string if reported
            if (nExIds_ >= 0x80000004)
            {
                memcpy(brand, extdata_[2].data(), sizeof(cpui));
                memcpy(brand + 16, extdata_[3].data(), sizeof(cpui));
                memcpy(brand + 32, extdata_[4].data(), sizeof(cpui));
                brand_ = brand;
            }
        };

        int nIds_;
        int nExIds_;
        std::string vendor_;
        std::string brand_;
        bool isIntel_;
        bool isAMD_;
        std::bitset<32> f_1_ECX_;
        std::bitset<32> f_1_EDX_;
        std::bitset<32> f_7_EBX_;
        std::bitset<32> f_7_ECX_;
        std::bitset<32> f_81_ECX_;
        std::bitset<32> f_81_EDX_;
        std::vector<std::array<int, 4>> data_;
        std::vector<std::array<int, 4>> extdata_;
    };
};

// Initialize static member data
const InstructionSet::InstructionSet_Internal InstructionSet::CPU_Rep;


using string = std::string;
int initFlags(ProcessorCoreDescriptor& desc) {
    int32_t state = 0;
    state = state | (InstructionSet::_3DNOW() << 0) | (InstructionSet::_3DNOWEXT() << 1) | (InstructionSet::ABM() << 2) | (InstructionSet::ADX() << 3) | (InstructionSet::AES() << 4) | (InstructionSet::AVX() << 5) | (InstructionSet::AVX2() << 6) | (InstructionSet::AVX512CD() << 7) | (InstructionSet::AVX512ER() << 8) | (InstructionSet::AVX512F() << 9) | (InstructionSet::AVX512PF() << 10) | (InstructionSet::BMI1() << 11) | (InstructionSet::BMI2() << 12) | (InstructionSet::CLFSH() << 13) | (InstructionSet::CMPXCHG16B() << 14) | (InstructionSet::CX8() << 15) | (InstructionSet::ERMS() << 16) | (InstructionSet::F16C() << 17) | (InstructionSet::FMA() << 18) | (InstructionSet::FSGSBASE() << 19) | (InstructionSet::FXSR() << 20) | (InstructionSet::HLE() << 21) | (InstructionSet::INVPCID() << 22) | (InstructionSet::LAHF() << 23) | (InstructionSet::LZCNT() << 24) | (InstructionSet::MMX() << 25) | (InstructionSet::MMXEXT() << 26) | (InstructionSet::MONITOR() << 27) | (InstructionSet::MOVBE() << 28) | (InstructionSet::MSR() << 29) | (InstructionSet::OSXSAVE() << 30) | (InstructionSet::PCLMULQDQ() << 31);
    int32_t state1 = (InstructionSet::POPCNT()) | (InstructionSet::PREFETCHWT1() << 1) | (InstructionSet::RDRAND() << 2) | (InstructionSet::RDSEED() << 3) | (InstructionSet::RDTSCP() << 4) | (InstructionSet::RTM() << 5) | (InstructionSet::SEP() << 6) | (InstructionSet::SHA() << 7) | (InstructionSet::SSE() << 8) | (InstructionSet::SSE2() << 9) | (InstructionSet::SSE3() << 10) | (InstructionSet::SSE41() << 11) | (InstructionSet::SSE42() << 12) | (InstructionSet::SSE4a() << 13) | (InstructionSet::SSSE3() << 14) | (InstructionSet::SYSCALL() << 15) | (InstructionSet::TBM() << 16) | (InstructionSet::XOP() << 17) | (InstructionSet::XSAVE() << 18);
    desc.flags0 = state;
    desc.flags1 = state1;
    desc.name = InstructionSet::Brand();
    desc.vendor = InstructionSet::Vendor();
    desc.ar = "x86";
    initialized = true;
    return 0;
}
