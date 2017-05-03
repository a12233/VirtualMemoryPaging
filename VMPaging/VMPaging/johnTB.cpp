#include "memoryManager.hpp"
#include <stdio.h>
using namespace std;


/*
Team: John Barczynski, Kyle Johnson, Mayank Makwana
Input from our tests folder is to be redirected to the executable for testing
*/
int main() {
	unsigned int virtualAddressSize = -1, numFrames = -1, N = -1, policy = -1;
	unsigned long long address;

	cout << "Policy Number (LRU = 1, FIFO=0):  ";
	cin >> policy;
	cout << policy << endl;
	cout << "Enter N: ";
	cin >> N;
	cout << N << endl << "Number of Frames: ";
	cin >> numFrames;
	cout << numFrames << endl << "Virtual Address Size: ";
	cin >> virtualAddressSize;
	cout << virtualAddressSize << endl << endl;
	memoryManager mem = memoryManager(policy ? LRU : FIFO, N, numFrames, virtualAddressSize);

	cout << "Give an address: ";
	while (cin >> address) {
		cout << "\nAddress: " << address << endl;
		cout << "Physical memory: " << mem.memoryAccess(address) << endl;
		cout << "Total Swaps: " << mem.numberPageSwaps() << endl << endl;
		cout << "Give an address: ";
	}
	cout << "quit program" << endl;
	return 0;
}