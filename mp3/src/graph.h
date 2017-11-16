#ifndef GRAPH_H
#define GRAPH_H

#include <string.h>
#include <stdlib.h>

#include <map>
#include <vector>
#include "node.h"
#include <iostream>
#include <algorithm>
#include <utility>

#include "parser.h"

using namespace std;

class Graph{
	private:
		int num_nodes;
		std::vector<Node> nodes;

	public:
		Graph();
		
		void print_nodes();
		vector<Node>::iterator node_in_graph(int);

		Node * add_node(int);
		Node * get_node(int);
		int remove_node(int);
		int add_edge(int, int, int);
		void update_edge(int, int, int);
		void remove_edge(int, int);

		void linkstate_init();
		void linkstate_init_node(Node & node);

		
		void distance_vector_init();
		bool distance_vector_process_node(Node * n);
		void set_init_distances_DV(map<int, pair<int,int> > & distances);

		void print_topology_entries();
		void apply_messages(char * filename);
		void apply_changes(char * filename, char * msg_file, bool ls_dv);
		vector<int> find_path(int, int);


};

#endif
