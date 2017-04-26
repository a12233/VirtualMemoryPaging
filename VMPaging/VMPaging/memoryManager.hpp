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
		virtualMemoryManagerInterface(p, pS, nF, vA) {};

	// Override
	unsigned long long memoryAccess(unsigned long long address) override;

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


