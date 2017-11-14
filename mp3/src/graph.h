#include <string.h>
#include <stdlib.h>

#include <map>
#include <vector>
#include "node.h"
#include <iostream>

using namespace std;

class Graph{

	int num_nodes;
	std::vector<Node> nodes;

	public:
		Graph();
		void print_nodes();
		bool node_in_graph(int);
		int add_node(int);
		int remove_node(int);
};