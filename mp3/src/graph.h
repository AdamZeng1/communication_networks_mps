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
		vector<Node>::iterator node_in_graph(int);
		Node * add_node(int);
		int remove_node(int);
		Node * get_node(int);
		int add_edge(int, int, int);
};