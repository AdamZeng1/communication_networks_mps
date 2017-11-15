#include "graph.h"

Graph::Graph(){
}
void Graph::linkstate_init(){
	for(vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		map<int, int> & distances = (*it).get_distances();
		distances.clear();
	}
	for(vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		Node n = (*it);
		linkstate_init_node(n);
		cout << "distances for node: " << n.get_id() << endl;
		map<int, int> d = n.get_distances();
		for (auto p: d){
			cout << "dest: " << p.first << " cost: " << p.second << endl;
		}
	}
	return;
}

void Graph::linkstate_init_node(Node & node){
	map<int, int> & distances = node.get_distances();
	map<int, int> & neighbors = node.get_neighbors();

	for (vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it){
		Node v = (*it);
		if (neighbors.count(v.get_id())){
			distances[v.get_id()] = neighbors[v.get_id()];
		}
		else{
			distances[v.get_id()] = 99999999;
		}
	}

	vector<int> visited;
	visited.push_back(node.get_id());
	int counter = 0;
	while(counter != distances.size() && visited.size() != distances.size()){
		int min_node = -1;
		int min_cost = 99999999;
		for (auto it_d: distances){
			if(find(visited.begin(), visited.end(), it_d.first) == visited.end()){
				if (it_d.second < min_cost){
					min_cost = it_d.second;
					min_node = it_d.first;
				}
			}
		}
		cout << min_cost << endl;
		visited.push_back(min_node);
		Node * w = this->get_node(min_node);
		map<int, int> w_neighbors = w->get_neighbors();
		for (auto w_d: w_neighbors){
			if(find(visited.begin(), visited.end(), w_d.first) == visited.end()){
				if (distances[w_d.first] > distances[min_node] + w_d.second){
					distances[w_d.first] = distances[min_node] + w_d.second;
				}
			}
		}

		counter++;
	}

	return;
}

void Graph::print_nodes(){
	for(vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		cout << "costs for node num: " << it->get_id() << endl;
		map<int, int> costs = it->get_neighbors();
		for (auto it: costs){
			cout << "dest: " << it.first << " cost: " << it.second << endl;
		}
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