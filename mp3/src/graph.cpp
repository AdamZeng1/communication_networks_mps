#include "graph.h"

Graph::Graph(){
}

void Graph::apply_messages(char * filename){
	ifstream msg_file;
	msg_file.open(filename);

	ofstream out_file;
	out_file.open("output.txt", ofstream::out | ofstream::app);
	string out_string = "";

	if (!msg_file){
		return;
	}
	string line;
	while (getline(msg_file, line)){
		int src_id, dest_id;
		string msg;
		getMsgNumbers(line, &src_id, &dest_id, &msg);
		Node * n = this->get_node(src_id);
		int path_cost = (n->get_distances())[dest_id].second;
		if (path_cost == 99999999){
			//cout << "from " << src_id << " to " << dest_id << " cost infinite hops unreachable message " << msg << endl;
			out_string = "from " + to_string(src_id) + " to " + to_string(dest_id) + " cost infinite hops unreachable message " + msg + "\n";
			out_file << out_string;
			continue;
		}
		else{
			//cout << "from " << src_id << " to " << dest_id << " cost " << path_cost << " hops ";
			out_string = "from " + to_string(src_id) + " to " + to_string(dest_id) + " cost " + to_string(path_cost) + " hops ";
			out_file << out_string;
			Node * cur_node = n;
			while(cur_node->get_id() != dest_id){
				//cout << cur_node->get_id() << " ";
				out_string = to_string(cur_node->get_id()) + " ";
				out_file << out_string;
				int next_node = (cur_node->get_distances())[dest_id].first;
				cur_node = this->get_node(next_node);
			}
			//cout << "message " << msg << endl;
			out_string = "message " + msg + "\n";
			out_file << out_string;
		}
	}
	out_file.close();
	return;
}

void Graph::apply_changes(char * filename, char * msg_file, bool ls_dv){
	ifstream changes_file;
	changes_file.open(filename);

	ofstream out_file;
	out_file.open("output.txt", ofstream::out | ofstream::app);

	if (!changes_file){
		return;
	}
	string line;
	while (getline(changes_file, line)){
		int src_id, dest_id, cost_change;
		getChangeNumbers(line, &src_id, &dest_id, &cost_change);
		if (cost_change == -999){
			this->remove_edge(src_id, dest_id);
		}
		else{
			this->update_edge(src_id, dest_id, cost_change);
		}
		if (ls_dv){
			this->linkstate_init();
		}
		else{
			this->distance_vector_init();
		}
		this->print_topology_entries();
		this->apply_messages(msg_file);
	}
	out_file.close();
	return;
}

void Graph::print_topology_entries(){
	int min_num = 99999999;
	Node * min_node = NULL;
	int visited_nodes = 0;

	ofstream out_file;
	out_file.open("output.txt", ofstream::out | ofstream::app);
	string out_string = "";

	while(visited_nodes != this->num_nodes){
		for(vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
			if ((*it).get_id() < min_num && (*it).printed == false){
				min_num = (*it).get_id();
				min_node = &(*it);
			}
		}
		if(min_node == NULL){
			break;
		}
		// print topology info for min_node: destination | nexthop | pathcost
		//cout << "<topology entries for node: " << min_num << ">" << endl;
		map<int, pair<int, int> > distances = min_node->get_distances();
		// sort distances
		for (auto d: distances){
			if (d.first == min_num){
				//cout << d.first << " " << min_num << " " << 0<< endl;
				out_string = to_string(d.first) + " " + to_string(min_num) + " 0\n";
				out_file << out_string;
			}
			else{
				//cout << d.first << " " << d.second.first << " " << d.second.second << endl;
				out_string = to_string(d.first) + " " + to_string(d.second.first) + " " + to_string(d.second.second) + "\n";
				out_file << out_string;
			}
		}
		min_node->printed = true;
		min_num = 99999999;
		min_node = NULL;
		visited_nodes += 1;
	}
	out_file.close();
	return;
}

void Graph::linkstate_init(){
	for(vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		map<int, pair<int, int> > & distances = (*it).get_distances();
		distances.clear();
		(*it).printed = false;
	}
	for(vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		linkstate_init_node((*it));
	}
	/*for(vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		Node n = (*it);
		map<int, pair<int, int> > distances = n.get_distances();
		cout << "distances for node num: "<<  n.get_id() << endl;
		for (auto d: distances){
			cout << d.first << " " << d.second.first << " " << d.second.second << endl;
		}
	}*/
	return;
}

void Graph::linkstate_init_node(Node & node){
	map<int, pair<int, int> > & distances = node.get_distances();
	map<int, int> & neighbors = node.get_neighbors();

	for (vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it){
		Node * v = &(*it);
		if (neighbors.count(v->get_id())){
			distances[v->get_id()] = make_pair(v->get_id(), neighbors[v->get_id()]);
		}
		else{
			distances[v->get_id()] = make_pair(-1, 99999999);
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
				if (it_d.second.second < min_cost){
					min_node = it_d.first;
					min_cost = it_d.second.second;
				}
				else if (it_d.second.second == min_cost){
					if(it_d.first < min_node){
						min_node = it_d.first;
						min_cost = it_d.second.second;
					}
				}

			}
		}
		visited.push_back(min_node);
		Node * w = this->get_node(min_node);
		map<int, int> w_neighbors = w->get_neighbors();
		for (auto w_d: w_neighbors){
			if(find(visited.begin(), visited.end(), w_d.first) == visited.end()){
				if(distances[min_node].second == 99999999 || w_d.second == 99999999){
					continue;
				}
				if (distances[w_d.first].second > distances[min_node].second + w_d.second){
					distances[w_d.first] = make_pair(distances[min_node].first, distances[min_node].second + w_d.second);
				}
				if (distances[w_d.first].second == distances[min_node].second + w_d.second){
					distances[w_d.first] = make_pair(distances[min_node].first, distances[min_node].second + w_d.second);
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
		Node * n = &(*it);
		n->printed = false;
		map<int, pair<int, int> > & distances = n->get_distances();
		messaged[n->get_id()] = true;
		this->set_init_distances_DV(distances);
		for (auto neighbor : n->get_neighbors()){
			distances[neighbor.first] = make_pair(neighbor.first, neighbor.second);
		}
	}

	while(still_converging){
		still_converging = false;
		for (auto m: messaged){
			new_messages[m.first] = false;
		}
		for(auto m : messaged) {
			if (m.second){
				Node * n = this->get_node(m.first);
				for(auto neighbor : n->get_neighbors()){
					bool new_message = distance_vector_process_node(this->get_node(neighbor.first));
					if (new_message){
						new_messages[neighbor.first] = true;
						still_converging = true;
					}
				}
			}
		}
		messaged = new_messages;
		new_messages.clear();
	}
	/*for (vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it){
		map<int, pair<int, int> > distances = (*it).get_distances();
		cout << "distances for node num: "<<  (*it).get_id() << endl;
		for (auto d: distances){
			cout << d.first << " " << d.second.first << " " << d.second.second << endl;
		}
	}*/
	return;
}

bool Graph::distance_vector_process_node(Node * n){
	map <int, pair<int, int> > & distances = n->get_distances();
	bool changed = false;
	for (auto d : distances) {
		if (d.first == n->get_id()){
			continue;
		}
		for (auto neighbor : n->get_neighbors()){
			Node * v = this->get_node(neighbor.first);
			map<int, pair<int, int> > neighbor_d = v->get_distances();
			if (neighbor_d[d.first].second == 99999999){
				continue;
			}
			int neighbor_cost_to_d = neighbor_d[d.first].second + distances[v->get_id()].second;
			if ( neighbor_cost_to_d < distances[d.first].second){
				distances[d.first] = make_pair(distances[neighbor.first].first, neighbor_cost_to_d);
				changed = true;
			}
			else if ( neighbor_cost_to_d == distances[d.first].second){
				if ( distances[neighbor.first].first < distances[d.first].first ) {
					distances[d.first] = make_pair(distances[neighbor.first].first, neighbor_cost_to_d);
					changed = true;
				}
				else if (distances[neighbor.first].first == distances[d.first].first){
					vector<int> path_1 = this->find_path(n->get_id(), d.first);
					vector<int> path_2 = this->find_path(v->get_id(), d.first);

					int last_hop_1 = path_1[path_1.size() - 1];
					int last_hop_2 = path_1[path_2.size() - 1];

					if (last_hop_2 < last_hop_1){
						distances[d.first] = make_pair(distances[neighbor.first].first, neighbor_cost_to_d);
						changed = true;
					}
				}
			}
		}
	}
	return changed;
}

vector<int> Graph::find_path(int start_id, int dest_id){
	Node * cur_node = this->get_node(start_id);
	vector<int> path;
	while(cur_node->get_id() != dest_id){
				path.push_back(cur_node->get_id());
				int next_node = (cur_node->get_distances())[dest_id].first;
				cur_node = this->get_node(next_node);
	}
	return path;
}

void Graph::set_init_distances_DV(map<int, pair<int, int> > & distances){
	distances.clear();
	for(vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		distances[(*it).get_id()] = make_pair(-1, 99999999);
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

void Graph::remove_edge(int n1, int n2){
	Node * n_1 = this->get_node(n1);
	Node * n_2 = this->get_node(n2);

	n_1->remove_edge(n2);
	n_2->remove_edge(n1);
	return;
}

void Graph::update_edge(int n1, int n2, int new_cost){
	Node * n_1 = this->get_node(n1);
	Node * n_2 = this->get_node(n2);

	n_1->update_edge(n2, new_cost);
	n_2->update_edge(n1, new_cost);
	return;
}

