#include "node.h"

using namespace std; 

Node::Node(int num) : id(num) {}

int Node::get_id(){
	return this->id;
}

int Node::add_edge(int id, int cost){	
	if(!this->distances.count(id)){
		this->distances[id] = cost;
		return 1;
	}
	return 0;
}

int Node::remove_edge(int id){
	return this->distances.erase(id);
}

int Node::update_edge(int id, int cost){
	this->distances[id] = cost;
	return 1;
}

int Node::get_cost(int id){
	if(this->distances.count(id)){
		return this->distances[id];
	}
	return -1;
}

map<int, int> Node::get_distances(){
	return this->distances;
}


