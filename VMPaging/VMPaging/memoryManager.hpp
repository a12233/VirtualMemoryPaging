#pragma once
#include <iostream>
#include <cassert>
#include <cmath>

#include "virtualMemoryManagerInterface.hpp"

using namespace std;

class memoryManager: public virtualMemoryManagerInterface{

public:
	// Constructor
	memoryManager(ReplacementPolicy p, unsigned int pS, unsigned int nF, unsigned int vA) :
		virtualMemoryManagerInterface(p, pS, nF, vA) {
		// initialize page table
			for (unsigned int i = 0; i < nF; i++) {
				PAGE_TABLE[i] = 0;
				// no memory-page assigned == false
				PAGE_TABLE_VALID[i] = false;
			}

			// initialize physical memory
			phyMemSize = numFrames * pow(2 , N);
			for (int i = 0; i < phyMemSize; i++) {
				PHYSICAL_MEMORY[i] = 0;
				PHYSICAL_MEMORY_FREE[i] = true;
				PHYSICAL_MEMORY_TIME_IN[i] = 0;
				PHYSCIAL_MEMORY_TIME_ACCESS[i] = 0;
			}
	
	};

	// Override
	unsigned long long memoryAccess(unsigned long long address) override;

	// Self-defined functions
	int findNextAvailableAddr();
	int findFifoAddr();
	int findLruAddr();
	int findPhysicalAddr(int addr);
	int findPageIndex(unsigned long long addr);
	int getPMIndex(int addr, int phyaddr);
	void timerUpdate(int phy_addr, bool first_in);

private:
	unsigned int * PAGE_TABLE;
	bool *PAGE_TABLE_VALID;

	unsigned int * PHYSICAL_MEMORY;
	bool * PHYSICAL_MEMORY_FREE;
	int * PHYSICAL_MEMORY_TIME_IN;
	int * PHYSCIAL_MEMORY_TIME_ACCESS;

	int phyMemSize;
	int Timer = 0;


};


