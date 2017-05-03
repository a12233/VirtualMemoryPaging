#pragma once
#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>

#include "virtualMemoryManagerInterface.hpp"

using namespace std;

class memoryManager: public virtualMemoryManagerInterface{

public:
	// Constructor
	memoryManager(ReplacementPolicy p, unsigned int pS, unsigned int nF, unsigned int vA) :
		virtualMemoryManagerInterface(p, pS, nF, vA) {
		// initialize page table

			virtualAddressSpaceSize = pow(2, vA);

			phyMemSize = numFrames; 
		 
			//PHYSICAL_MEMORY = (int*)calloc(numFrames, 8); 
			//PHYSICAL_MEMORY_TIME_IN = (int*)calloc(numFrames, 8);
			//PHYSCIAL_MEMORY_TIME_ACCESS = (int*)calloc(numFrames, 8);
			//PHYSICAL_MEMORY_FREE = (bool*)calloc(numFrames, 1);

			for (int i = 0; i < phyMemSize; i++) {
			//	PHYSICAL_MEMORY_FREE[i] = true;
			//	PHYSICAL_MEMORY[i] = -1;
			//	PHYSCIAL_MEMORY_TIME_ACCESS[i] = -1;
			//	PHYSICAL_MEMORY_TIME_IN[i] = -1;

				phyMem.push_back(-1);
				phyMemFree.push_back(true);
				phyMemTimeIn.push_back(-1);
				phyMemTimeAcc.push_back(-1);
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
	int getPMIndex(int addr, int frame_i);
	void timerUpdate(int phy_addr, bool first_in);
	void freeMem(); 

private:

	vector<int> phyMem;
	vector<bool> phyMemFree;
	vector<int> phyMemTimeIn;
	vector<int> phyMemTimeAcc;


	int phyMemSize;
	int Timer = 0;


};


