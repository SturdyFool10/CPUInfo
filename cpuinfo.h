#pragma once
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <syncstream>
bool initialized = false;
#ifdef _M_AMD64
#define arch "x86"
#elif defined _M_IX86
#define arch "x86"
#elif defined _M_ARM
#define arch "ARM"
#elif defined _M_ARM64
#define arch "ARM"
#else
#error "SturdyEngine does not support your archetechure, please compile for either x86-64 or for ARM"
#endif
#ifdef _WIN32
#include <windows.h>
#endif
#include <syncstream>


class ProcessorCoreDescriptor {
public:
	std::string ar;
	int32_t flags0 = 0x0000;
	int32_t flags1 = 0x0000;
	std::string name;
	std::string vendor;
	bool getFlag0(int shift) {
		return (flags0 & shift) != 0;
	}
	bool getFlag1(int shift) {
		return (flags1 & shift) != 0;
	}
private:
	void setArch(std::string ar) {
		this->ar = ar;
	}
};


#ifdef _M_AMD64
#include "cpuinfox86.h"
#define cpuLibIncluded true
#elif defined _M_IX86
#ifndef cpuLibIncluded
#include "cpuinfox86.h"
#define cpuLibIncluded true
#endif
#elif defined _M_ARM
#ifndef cpuLibIncluded
#include "cpuinfoARM.h"	
#define cpuLibIncluded true
#endif
#elif defined _M_ARM64
#ifndef cpuLibIncluded
#include "cpuinfoARM.h"	
#define cpuLibIncluded true
#endif
#else
#error "SturdyEngine does not support your archetechure, please compile for either x86-64 or for ARM"
#endif
class ProcessorDescriptor {
	std::vector<std::shared_ptr<std::mutex>> threadMutexes;
	std::vector<ProcessorCoreDescriptor> coreDescriptors;
	std::vector<bool> threadsFinished;
public:
	int numThreads;
	ProcessorDescriptor() {
		
		numThreads = std::thread::hardware_concurrency();
		threadMutexes.resize(numThreads);
		coreDescriptors.resize(numThreads);
		std::osyncstream(std::cout) << "Checking hardware..." << std::endl;
		threadsFinished.resize(numThreads);
		for (int i = 0; i < numThreads; ++i) {
			// Allocate a new mutex for each thread
			threadMutexes[i] = std::make_shared<std::mutex>();
			coreDescriptors[i] = ProcessorCoreDescriptor();
			// Create a thread with specific core affinity
			threadsFinished[i] = false;
			auto lambd = [this, i]() {
				auto lock = std::lock_guard<std::mutex>(*this->threadMutexes[i]);
				auto& desc = this->coreDescriptors[i];
				initFlags(desc);
				threadsFinished[i] = true;
			};
			{
				std::thread t(lambd); // Perfect forwarding of the callable

#ifdef _WIN32
				//Windows-specific code to set thread affinity
				HANDLE threadHandle = t.native_handle();
				DWORD_PTR affinityMask = 1ULL << i;
				if (SetThreadAffinityMask(threadHandle, affinityMask) == 0) {
					std::cerr << "Failed to set thread affinity on Windows. Error: " << GetLastError() << std::endl;
				}
#else
				// Linux-specific code to set thread affinity
				cpu_set_t cpuset;
				CPU_ZERO(&cpuset);
				CPU_SET(coreId, &cpuset);
				int rc = pthread_setaffinity_np(t.native_handle(), sizeof(cpu_set_t), &cpuset);
				if (rc != 0) {
					std::cerr << "Failed to set thread affinity on Linux. Error: " << strerror(rc) << std::endl;
				}
#endif
				t.detach();
			}
		}
		for (int i = 0; i < numThreads; ++i) {
			while (!threadsFinished[i]) {}
		}
	}
	ProcessorCoreDescriptor getCoreDescriptor(int index) {
		return this->coreDescriptors[index];
	}
};

namespace CPUInfo {
	class CPU {
	public:
		bool isHybrid = false; //assume non-hybrid
		CPU() {
			if (!initialized) {
				this->proc = ProcessorDescriptor();
				for (int i = 0; i < proc.numThreads; ++i) {
					auto desc = proc.getCoreDescriptor(i);
					for (int j = 0; j < proc.numThreads; ++j) {
						auto desc2 = proc.getCoreDescriptor(j);
						if (!(desc.flags0 == desc2.flags0 && (desc.flags1 == desc2.flags1))) {
							this->isHybrid = true; //if the flags of every core are not the same, tell the struct we are a hybrid CPU
						}
					}
				}
			}
		}
		ProcessorDescriptor& getDescriptor() {
			return this->proc;
		}
	private:
		ProcessorDescriptor proc;
	};
}
