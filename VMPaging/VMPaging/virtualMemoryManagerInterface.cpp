// CMPSC 473: Project3

#include "virtualMemoryManagerInterface.hpp"

virtualMemoryManagerInterface::virtualMemoryManagerInterface(ReplacementPolicy p, unsigned int pS, unsigned int nF, unsigned int vA) :
	policy(p), N(pS), numFrames(nF), virtualAddressSpaceSize(vA) {

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

/** This is the method the test bench will drive in emulating memory management.
*	Your memory manager should return the physical address corresponding to the given virtual
*	address. This function must NOT return until any page swapping is completed, if necessary.
*	This function is to effect page swaps by calling the other key function (defined below)
*/
unsigned long long virtualMemoryManagerInterface::memoryAccess(unsigned long long address) {

	// If not a valid addr, reject
	if (address > virtualAddressSpaceSize) {
		cerr << "invalid virtual address!" << endl;
	}

	// return addr if required has already been in the physical memory
	if (PAGE_TABLE_VALID[address] == true) {
		timerUpdate(PAGE_TABLE[address],false);

		return PAGE_TABLE[address];
	}

	// if not in the memory
	// try to find a place available in the memory
	int nextAvailableAddr = findNextAvailableAddr();
	if (nextAvailableAddr != -1) {
		PAGE_TABLE[address] = nextAvailableAddr;
		PAGE_TABLE_VALID[address] = true;

		timerUpdate(PAGE_TABLE[address],true);

		return nextAvailableAddr;
	}

	// if no place available in the memory, execute the replacement policy
	if (policy == FIFO) {
		nextAvailableAddr = findFifoAddr();
	}
	else {
		nextAvailableAddr = findLruAddr();
	}
	// Save back to disk
	swap(nextAvailableAddr, address);
	PAGE_TABLE[address] = nextAvailableAddr;
	PAGE_TABLE_VALID[address] = true;

	timerUpdate(PAGE_TABLE[address],true);
	return nextAvailableAddr;
}

// Update timer, PHYSICAL_MEMORY_TIME_IN, PHYSCIAL_MEMORY_TIME_ACCESS
void virtualMemoryManagerInterface::timerUpdate(int phy_addr,bool first_in) {

	PHYSCIAL_MEMORY_TIME_ACCESS[phy_addr] = Timer;
	if (first_in) {
		PHYSICAL_MEMORY_TIME_IN[phy_addr] = Timer;
	}
	Timer++;
}

/** This is the method to find index of the next free spot among the Physical memory
* Return -1 if all physical memory occupied
*/
int virtualMemoryManagerInterface::findNextAvailableAddr() {

	for (int i = 0; i < phyMemSize; i++) {
		if (PHYSICAL_MEMORY_FREE[i] == true) {
			PHYSICAL_MEMORY_FREE[i] = false;
			return i;
		}
	}
	return (-1);
}


/** This is the method to find FIFO index among the Physical memory
*/
int virtualMemoryManagerInterface::findFifoAddr() {
	int fifo_time = INFINITY;
	int fifo_index = -1;

	for (int i = 0; i < phyMemSize; i++) {
		if (PHYSICAL_MEMORY_TIME_IN[i] < fifo_time) {
			fifo_time = PHYSICAL_MEMORY_TIME_IN[i];
			fifo_index = i;
		}
	}
	return fifo_index;
}

/** This is the method to find LRU index among the Physical memory
*/
int virtualMemoryManagerInterface::findLruAddr() {
	int lru_time = INFINITY;
	int lru_index = -1;

	for (int i = 0; i < phyMemSize; i++) {
		if (PHYSCIAL_MEMORY_TIME_ACCESS[i] < lru_time) {
			lru_time = PHYSICAL_MEMORY_TIME_IN[i];
			lru_index = i;
		}
	}
	return lru_index;
}
