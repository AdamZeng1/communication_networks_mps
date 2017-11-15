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

void Graph::distance_vector_init(){
	map<int, bool> messaged;
	map<int, bool> new_messages;
	bool still_converging = true;
	for(vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		Node n = (*it);
		map<int, int> & distances = n.get_distances();
		messaged[n.get_id()] = true;
		this->set_init_distances_DV(distances);
		for (auto neighbor : n.get_neighbors()){
			distances[neighbor.first] = neighbor.second;
		}
	}

	while(still_converging){
		still_converging = false;
		for(auto m : messaged) {
			if(m.second){
				Node * n = this->get_node(m.first);
				for(auto neighbor : n->get_neighbors()){
					bool new_message = distance_vector_process_node(*(this->get_node(neighbor.first)));
					new_messages[m.first] = new_message;
					if (new_message){
						still_converging = true;
					}
				}
			}
			else{
				new_messages[m.first] = false;
			}
		}
		messaged = new_messages;
		new_messages.clear();
	}
	return;
}

bool Graph::distance_vector_process_node(Node n){
	map <int, int> distances = n.get_distances();
	bool changed = false;
	for (auto d : distances)	
		for (auto neighbor : n.get_neighbors()){
			Node * v = this->get_node(neighbor.first);
			map<int, int> neighbor_d = v->get_distances();
			int neighbor_cost_to_d = neighbor_d[d.first] + distances[v->get_id()];
			if ( neighbor_cost_to_d < d.second){
				distances[v->get_id()] = neighbor_cost_to_d;
				changed = true;
			}
		}
		return changed;
}

void Graph::set_init_distances_DV(map<int, int> & distances){
	distances.clear();
	for(vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		distances[(*it).get_id()] = 99999999;
	}
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