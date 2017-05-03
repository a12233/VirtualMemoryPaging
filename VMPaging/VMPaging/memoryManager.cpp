#include "memoryManager.hpp"


unsigned long long memoryManager::memoryAccess(unsigned long long address) {

	//cout << "virtual address: " << address << endl;
	unsigned long long pageNum = findPageIndex(address);
	//cout << "virtual page: " << pageNum << endl;
	
	// If not a valid addr, reject
	if (pow(2,pageNum) > pow(2,virtualAddressSpaceSize)) {
		cerr << "invalid virtual address!" << endl;
	}

	// Check whether the page has already been in the memory
	int nextAvailableFrame = findPhysicalAddr(pageNum);

	// if in memory, return
	if (nextAvailableFrame != -1) {
		timerUpdate(nextAvailableFrame, false);
		//cout << "physical frame: " << nextAvailableFrame << endl;
		//cout << "PHYSCIAL addr: " << getPMIndex(address, nextAvailableFrame) << endl;
		return getPMIndex(address,nextAvailableFrame);
	}
	
	// if not in the memory
	// if any free frame in memory, take the free spot
	nextAvailableFrame = findNextAvailableAddr();
	// if no place available in the memory, execute the replacement policy
	if (nextAvailableFrame == -1) {
		if (policy == FIFO) {   
			nextAvailableFrame = findFifoAddr();
			if (nextAvailableFrame == -1) {
				cout << "FIFO wrong!" << endl;
			}
		}
		else {
			nextAvailableFrame = findLruAddr();
			if (nextAvailableFrame == -1) {
				cout << "LRU wrong!" << endl;
			}
		}
		// Save back to disk
	//	cout << "swap!!" << endl;
		swap(phyMem[nextAvailableFrame], pageNum); 
	}
	// Set page into frame
	phyMem[nextAvailableFrame] = pageNum; 
	phyMemFree[nextAvailableFrame] = false; 
	timerUpdate(nextAvailableFrame, true);

//	cout << "physical frame: " << nextAvailableFrame << endl;
//	cout << "PHYSCIAL addr: " << getPMIndex(address, nextAvailableFrame) << endl;

	return getPMIndex(address, nextAvailableFrame);

}

int memoryManager::findPageIndex(unsigned long long addr) {

	int page = addr / pow(2,N); 
	return page; 

}

int memoryManager::getPMIndex(int addr, int frame_i) {
	int offset = addr % (int)pow(2, N);
	return (frame_i*pow(2,N) + offset);
}

int memoryManager::findPhysicalAddr(int addr) {
	for (int i = 0; i < phyMemSize; i++) { 
		//if (PHYSICAL_MEMORY[i] == addr) {
		if(phyMem[i] == addr) {
			return i;
		}
	}
	return (-1);
}

// Update timer, PHYSICAL_MEMORY_TIME_IN, PHYSCIAL_MEMORY_TIME_ACCESS
void memoryManager::timerUpdate(int phy_addr, bool first_in) {

	phyMemTimeAcc[phy_addr] = Timer; 
	//PHYSCIAL_MEMORY_TIME_ACCESS[phy_addr] = Timer;
	if (first_in) {
		//PHYSICAL_MEMORY_TIME_IN[phy_addr] = Timer;
		phyMemTimeIn[phy_addr] = Timer; 
	}
	Timer++;
}

/** This is the method to find index of the next free spot among the Physical memory
* Return -1 if all physical memory occupied
*/
int memoryManager::findNextAvailableAddr() {

	for (int i = 0; i < phyMemSize; i++) {
		//if (PHYSICAL_MEMORY_FREE[i] == true) {
		//	PHYSICAL_MEMORY_FREE[i] = false;
		if(phyMemFree[i] == true) {
			phyMemFree[i] = false; 
			return i;
		}
	}
	return (-1);
}


/** This is the method to find FIFO index among the Physical memory
*/
int memoryManager::findFifoAddr() {
	int fifo_time = 10000000;
	int fifo_index = -1;

	for (int i = 0; i < phyMemSize; i++) {
		//if (PHYSICAL_MEMORY_TIME_IN[i] < fifo_time) {
		//	fifo_time = PHYSICAL_MEMORY_TIME_IN[i];
		if(phyMemTimeIn[i] < fifo_time) {
			fifo_time = phyMemTimeIn[i]; 
			fifo_index = i;
		}
	}
	return fifo_index;
}

/** This is the method to find LRU index among the Physical memory
*/
int memoryManager::findLruAddr() {
	int lru_time = 10000000;
	int lru_index = -1;

	for (int i = 0; i < phyMemSize; i++) {
		//if (PHYSCIAL_MEMORY_TIME_ACCESS[i] < lru_time) {
			//lru_time = PHYSICAL_MEMORY_TIME_IN[i];

		if(phyMemTimeAcc[i]<lru_time) {
			lru_time = phyMemTimeAcc[i]; 
			lru_index = i;
		}
	}
	return lru_index;
}
void memoryManager::freeMem()
{
	//free(PAGE_TABLE);
	//free(PAGE_TABLE_VALID);
	//free(PHYSICAL_MEMORY);
	//free(PHYSICAL_MEMORY_FREE);
	//free(PHYSICAL_MEMORY_TIME_IN);
	//free(PHYSCIAL_MEMORY_TIME_ACCESS);

}