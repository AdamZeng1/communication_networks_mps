#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include <iostream>
#include <fstream>
#include "parser.h"

using namespace std;

int main(int argc, char** argv) {
	//printf("Number of arguments: %d", argc);
	if (argc != 4) {
		printf("Usage: ./distvec topofile messagefile changesfile\n");
		return -1;
	}
	char * topo_file = argv[1];
	char * msg_file = argv[2];
	char * changes_file = argv[3];

	ofstream out_file;
	out_file.open("output.txt");
	out_file << "";
	out_file.close();

	Graph g = Graph();
	parseTopology(topo_file, &g);
	g.distance_vector_init();
	g.print_topology_entries();
	g.apply_messages(msg_file);
	g.apply_changes(changes_file, msg_file, false);
	return 0;
}

