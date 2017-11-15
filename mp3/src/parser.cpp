#include "parser.h"

void getTopoNumbers(string line, int * src_id, int * dest_id, int * cost){
	int first_space = line.find(" ");
	*src_id = stoi(line.substr(0, first_space));
	int second_space = line.find(" ", first_space + 1);
	*dest_id = stoi(line.substr(first_space + 1, second_space));
	*cost = stoi(line.substr(second_space + 1));
}

void getMsgNumbers(string line, int * src_id, int * dest_id, string * msg){
	int first_space = line.find(" ");
	*src_id = stoi(line.substr(0, first_space));
	int second_space = line.find(" ", first_space + 1);
	*dest_id = stoi(line.substr(first_space + 1, second_space));
	*msg = line.substr(second_space + 1);
}

void getChangeNumbers(string line, int * src_id, int * dest_id, int * cost_change){
	int first_space = line.find(" ");
	*src_id = stoi(line.substr(0, first_space));
	int second_space = line.find(" ", first_space + 1);
	*dest_id = stoi(line.substr(first_space + 1, second_space));
	*cost_change = stoi(line.substr(second_space + 1));
}

int parseTopology(char * filename, Graph * g){
	ifstream topo_file;
	topo_file.open(filename);

	if (!topo_file){
		return -1;
	}
	string line;
	while (getline(topo_file, line)){
		int src_id, dest_id, cost;
		getTopoNumbers(line, &src_id, &dest_id, &cost);
		g->add_node(src_id);
		g->add_node(dest_id);
		g->add_edge(src_id, dest_id, cost);
	}
	return 1;
}

