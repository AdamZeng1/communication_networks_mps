#include "graph.h"

Graph::Graph(){
}

void Graph::print_nodes(){
	for(vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		cout << it->get_id() << endl;
	}
}

bool Graph::node_in_graph(int id){
	for(vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		if(it->get_id() == id){
			return true;
		}
	}
	return false;
}
int Graph::add_node(int id){
	if(!this->node_in_graph(id)){
		this->nodes.push_back(Node(id));
		this->num_nodes++;
	}
	return this->num_nodes;
}
int Graph::remove_node(int id){
	for(vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		if(it->get_id() == id){
			this->nodes.erase(it);
			this->num_nodes--;
		}
	}
	return this->num_nodes;
}