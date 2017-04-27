

#include "memoryManager.hpp"
#include "virtualMemoryManagerInterface.hpp"


int main()
{
	//	memoryManager(ReplacementPolicy p, unsigned int pS, unsigned int nF, unsigned int vA) :
	//virtualMemoryManagerInterface(ReplacementPolicy p, unsigned int pS, unsigned int nF, unsigned int vA) : policy(p), N(pS), numFrames(nF), virtualAddressSpaceSize(vA) {
		//assert(virtualAddressSpaceSize > N);
		//assert(pow(2, virtualAddressSpaceSize) > numFrames * pow(2, N));
	
	int frames[12] = { 0,2,1,6,4,0,1,0,3,1,2,1 };
	memoryManager mem1(FIFO, 4, 4, 100);

	for (int i = 0; i < 12; i++)
	{
		mem1.memoryAccess(frames[i]);

	}
	cout << mem1.numberPageSwaps();
	mem1.freeMem();
	return 0;
}///pushing to github
