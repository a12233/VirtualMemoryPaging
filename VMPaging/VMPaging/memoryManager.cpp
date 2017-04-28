#include "memoryManager.hpp"


unsigned long long memoryManager::memoryAccess(unsigned long long address) {

	cout << "virtual address: " << address << endl;
	unsigned long long addressNew = findPageIndex(address);
	cout << "virtual page: " << addressNew << endl;
	// If not a valid addr, reject
	if (pow(2,addressNew) > pow(2,virtualAddressSpaceSize)) {
		cerr << "invalid virtual address!" << endl;
	}

	int phyAddrIdx = findPhysicalAddr(addressNew);
	// return addr if required has already been in the physical memory
	if (phyAddrIdx != -1) {
		timerUpdate(phyAddrIdx, false);
		cout << "physical frame: " << phyAddrIdx << endl;
		cout << "PHYSCIAL addr: " << getPMIndex(address, phyAddrIdx) << endl;
		return getPMIndex(addressNew,phyAddrIdx);
	}
	
	// if not in the memory
	// try to find a place available in the memory
	int nextAvailableAddr = findNextAvailableAddr();
	// if no place available in the memory, execute the replacement policy
	if (nextAvailableAddr == -1) {
		if (policy == FIFO) {   
			nextAvailableAddr = findFifoAddr();
			if (nextAvailableAddr == -1) {
				cout << "FIFO wrong!" << endl;
			}
		}
		else {
			nextAvailableAddr = findLruAddr();
			if (nextAvailableAddr == -1) {
				cout << "lRU wrong!" << endl;
			}
		}
		// Save back to disk
		swap(PHYSICAL_MEMORY[nextAvailableAddr], addressNew);
	}
	PHYSICAL_MEMORY[nextAvailableAddr] = addressNew;
	PHYSICAL_MEMORY_FREE[nextAvailableAddr] = false;
	timerUpdate(nextAvailableAddr, true);
	//freeMem();
	cout << "physical frame: " << nextAvailableAddr << endl;
	cout << "PHYSCIAL addr: " << getPMIndex(address, nextAvailableAddr) << endl;
	cout << endl;
	return getPMIndex(address, nextAvailableAddr);

}

int memoryManager::findPageIndex(unsigned long long addr) {
	/*
	unsigned long long index = address / PAGESIZE ;
	unsigned long long offset = address % PAGESIZE ;
	
	*/
	int page = addr / pow(2,N); 
	return page; 
	//double page = (pow(2,virtualAddressSpaceSize)) / (pow(2, N));
	//return floor(page);
}

int memoryManager::getPMIndex(int addr, int phyaddr) {

	int VMpage = findPageIndex(addr);
	int restAddr = addr - pow(2, VMpage);
	return (phyaddr*pow(2,VMpage)) + restAddr;

}

int memoryManager::findPhysicalAddr(int addr) {
	for (int i = 0; i < phyMemSize; i++) { 
		if (PHYSICAL_MEMORY[i] == addr) {
			return i;
		}
	}
	return (-1);
}

// Update timer, PHYSICAL_MEMORY_TIME_IN, PHYSCIAL_MEMORY_TIME_ACCESS
void memoryManager::timerUpdate(int phy_addr, bool first_in) {

	PHYSCIAL_MEMORY_TIME_ACCESS[phy_addr] = Timer;
	if (first_in) {
		PHYSICAL_MEMORY_TIME_IN[phy_addr] = Timer;
	}
	Timer++;
}

/** This is the method to find index of the next free spot among the Physical memory
* Return -1 if all physical memory occupied
*/
int memoryManager::findNextAvailableAddr() {

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
int memoryManager::findFifoAddr() {
	int fifo_time = 100000;
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
int memoryManager::findLruAddr() {
	int lru_time = 100000;
	int lru_index = -1;

	for (int i = 0; i < phyMemSize; i++) {
		if (PHYSCIAL_MEMORY_TIME_ACCESS[i] < lru_time) {
			lru_time = PHYSICAL_MEMORY_TIME_IN[i];
			lru_index = i;
		}
	}
	return lru_index;
}
void memoryManager::freeMem()
{
	free(PAGE_TABLE);
	free(PAGE_TABLE_VALID);
	free(PHYSICAL_MEMORY);
	free(PHYSICAL_MEMORY_FREE);
	free(PHYSICAL_MEMORY_TIME_IN);
	free(PHYSCIAL_MEMORY_TIME_ACCESS);

}