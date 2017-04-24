#pragma once
#include "virtualMemoryManagerInterface.hpp"

using namespace std;

class myVMMI : public virtualMemoryManagerInterface
{
public:
	myVMMI(ReplacementPolicy p, unsigned int pS, unsigned int nF, unsigned int vA) :
		virtualMemoryManagerInterface(p, pS, nF, vA) {

		// initialize page table
		for (int i = 0; i < N; i++) {
			PAGE_TABLE[i] = 0;
			// no memory-page assigned == false
			PAGE_TABLE_VALID[i] = false;
		}

		// initialize physical memory
		phyMemSize = numFrames * 2 ^ N;
		for (int i = 0; i < phyMemSize; i++) {
			PHYSICAL_MEMORY[i] = 0;
			PHYSICAL_MEMORY_FREE[i] = true;
			PHYSICAL_MEMORY_TIME_IN[i] = 0;
			PHYSCIAL_MEMORY_TIME_ACCESS[i] = 0;
		}



	}
	virtual unsigned long long memoryAccess(unsigned long long address) = 0;
	void swap(unsigned int frameNumber, unsigned int pageNumber) {
		///@todo ta instrumentation to go here
		///students should instrument memorymanager performance in their own class for their internal verification or may
		///modify this code for their testing purpose; however the TAs instrumentation will used for determining grade basis.
		numSwaps++;
	}


	// Self-defined functions
	int findNextAvailableAddr();
	int findFifoAddr();
	int findLruAddr();
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