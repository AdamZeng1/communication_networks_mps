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
		int num_collisions = 0;
		int num_transmissions = 0;
		int collision_count = 0;
		int num_drops = 0;
};