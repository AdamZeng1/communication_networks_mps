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
		printf("Usage: ./linkstate topofile messagefile changesfile\n");
		return -1;
	}
	char * topo_file = argv[1];
	char * msg_file = argv[2];
	char * changes_file = argv[3];

	Graph topo_graph = Graph();
	parseTopology(topo_file, &topo_graph);
	topo_graph.print_nodes();
	//int change_fxn = parseChanges(changes_file);

	ofstream out_file;
	out_file.open("output.txt");
	out_file.close();
	return 0;
}

