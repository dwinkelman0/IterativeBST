#include "ibst.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <map>

long GetTime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

int int_cmp(int a, int b) {
	if (a < b) return -1;
	else if (a == b) return 0;
	else return 1;
}

int main() {
	BST<int, int> * bst = new BST<int, int>();
	bst->compare_func = int_cmp;
	const unsigned int N_RANDOMS = 1000000;
	int randoms[N_RANDOMS];
	for (unsigned int i = 0; i < N_RANDOMS; i++) {
		randoms[i] = rand();
	}
	
	long t0 = GetTime();
	
	for (unsigned int i = 0; i < 1023; i++) {
		bst->Insert(randoms[i], i);
	}
	BST<int, int> * new_bst = bst->Rebalance();
	for (unsigned int i = 1023; i < N_RANDOMS; i++) {
		bst->Insert(randoms[i], i);
	}
	
	long t1 = GetTime();
	printf("Insertion (BST): %i milliseconds\n", (int)(t1 - t0));
	
	std::map<int, int> table;
	for (unsigned int i = 0; i < N_RANDOMS; i++) {
		table.insert(std::pair<int, int>(randoms[i], i));
	}
	
	long t2 = GetTime();
	printf("Insertion (Map): %i milliseconds\n", (int)(t2 - t1));
	
	unsigned int n_nodes;
	BST<int, int>::Node ** serialized = new_bst->Serialize(&n_nodes);
	
	long t3 = GetTime();
	printf("Serialization: %i milliseconds\n", (int)(t3 - t2));
	
	delete[] serialized;
	
	delete bst;
	delete new_bst;
	
	return 0;
}