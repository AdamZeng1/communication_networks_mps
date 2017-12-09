#include <string.h>
#include <stdlib.h>
#include <map>
#include <iostream>

#include <utility>
using namespace std; 

class Node {
	public:
		Node(int);
		int backoff;
		int num_collisions;
		int num_transmissions;
};