#include "node.h"

using namespace std; 

Node::Node(int num) : id(num) {}

int Node::get_id(){
	return this->id;
}

int Node::add_edge(int id, int cost){	
	if(!this->neighbors.count(id)){
		this->neighbors[id] = cost;
		return 1;
	}
	return 0;
}

int Node::remove_edge(int id){
	return this->neighbors.erase(id);
}

int Node::update_edge(int id, int cost){
	this->neighbors[id] = cost;
	return 1;
}

int Node::get_cost(int id){
	if(this->neighbors.count(id)){
		return this->neighbors[id];
	}
	return -1;
}

map<int, int> &  Node::get_neighbors(){
	return  (this->neighbors);
}

map<int, pair<int, int> > & Node::get_distances(){
	return (this->distances);
}

