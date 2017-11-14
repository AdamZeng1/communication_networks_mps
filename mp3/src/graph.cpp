#include "graph.h"

Graph::Graph(){
}

void Graph::print_nodes(){
	for(vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		cout << it->get_id() << endl;
	}
}

vector<Node>::iterator Graph::node_in_graph(int id){
	for(vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		if(it->get_id() == id){
			return it;
		}
	}
	return this->nodes.end();
}
Node * Graph::add_node(int id){
	Node * node = this->get_node(id);
	if(!node){
		this->nodes.push_back(Node(id));
		this->num_nodes++;
		node = this->get_node(id);
	}
	return node;
}
int Graph::remove_node(int id){
	vector<Node>::iterator it = this->node_in_graph(id);
	
	if(this->nodes.end() != it){
		this->nodes.erase(it);
		this->num_nodes--;
	}
	return this->num_nodes;
}

Node * Graph::get_node(int id){
	for(vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		if(it->get_id() == id){
			return &(*it);
		}
	}
	return NULL;
}

int Graph::add_edge(int id_1, int id_2, int cost){
	Node * node_1 = this->add_node(id_1);
	Node * node_2 = this->add_node(id_2);

	if(node_1 && node_2){
		node_1->add_edge(id_2, cost);
		node_2->add_edge(id_1, cost);
		return 1;
	}
	return 0;
}