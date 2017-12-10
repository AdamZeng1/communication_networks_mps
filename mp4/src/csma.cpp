#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>
#include <iostream>

#include <vector>

#include "node.h"

using namespace std;

int N = -1;
int L = -1;
vector<int> R;
int M = -1;
int T = -1;
bool channelOccupied = false;


// output stats
int num_collisions = 0;
int num_transmissions = 0; 
int num_idle = 0;  // in clock ticks


void getInputs(string input_file){
	int N_idx = input_file.find("N");
	int N_end = input_file.find("\n");
	N = stoi(input_file.substr(N_idx+2, N_end));

	int L_idx = input_file.find("L", N_end+1);
	int L_end = input_file.find("\n", N_end+1);
	L = stoi(input_file.substr(L_idx+2, L_end - L_idx - 2));


	int R_idx = input_file.find("R", L_end+1);
	int R_end = input_file.find("\n", L_end+1);
	string R_str = input_file.substr(R_idx+2, R_end - R_idx - 2);
	int idx = 0;
	int prev_space_idx = 0;

	while(true){
		int space_idx = R_str.find(" ", prev_space_idx+1);
		bool R_done = false;
		if (space_idx == -1){
			space_idx = R_str.find("\n", prev_space_idx+1);
			R_done = true;
		}
		int val = stoi(R_str.substr(idx, space_idx));
		R.push_back(val);
		prev_space_idx = space_idx;
		idx = space_idx+1;
		if (R_done){
			break;
		}
	}

	int M_idx = input_file.find("M", R_end+1);
	int M_end = input_file.find("\n", R_end+1);
	M = stoi(input_file.substr(M_idx+2, M_end - M_idx - 2));

	int T_idx = input_file.find("T", M_end+1);
	int T_end = input_file.find("\n", M_end+1);
	T = stoi(input_file.substr(T_idx+2, T_end - T_idx - 2));
	return;
}

int main(int argc, char** argv) {

	if (argc < 2){
		cout << "Please provide input file" << endl;
		return 0;
	}
	ifstream inFile;
	inFile.open(argv[1]);
	stringstream file_stream;
	file_stream << inFile.rdbuf();
	string input_file = file_stream.str();
	inFile.close();

	getInputs(input_file);

	vector<Node> nodes;

	for (int i =0; i < N; i++){
		Node node(rand() % R[0]);
		nodes.push_back(node);
	}
	int counter = 0;

	for (int clk = 0; clk < T; clk++){
		vector <Node *> ready_nodes;

		if (counter == 0){
			channelOccupied = false;
		}

		if (!channelOccupied){
			for(vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
				if (!(*it).backoff){
					ready_nodes.push_back(&(*it));
				}
			}
			if (ready_nodes.size() == 0){
				for(vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
					(*it).backoff--;
				}
			}
			else if (ready_nodes.size() == 1){
				channelOccupied = true;
				counter = L;
				Node * node = ready_nodes[0];
				node->num_transmissions++;
				node->collision_count = 0;
				node->backoff = rand() % R[0];
			}
			else{
				num_collisions += 1;
				for(vector<Node *>::iterator it = ready_nodes.begin(); it != ready_nodes.end(); ++it) {
					Node * node = (*it);
					node->collision_count++;
					if (node->collision_count == M){
						node->num_drops++;
						node->num_collisions++;
						node->collision_count = 0;
						node->backoff =  rand() % R[0];
					}
					else {
						if (node->collision_count >= R.size()){
							node->backoff = rand() % R[R.size() - 1];
						}
						else{
							node->backoff = rand() % R[node->collision_count];
						}
					}
				}
			}
		}
		else{
			counter--;
		}

	}

	int total_collisions = 0;
	float collision_sq = 0;
	float collision_mean;

	float transmission_sq = 0;
	float transmission_mean;

	for(vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
		Node * node = &(*it);
		total_collisions += node->num_collisions;
		num_transmissions += node->num_transmissions;
	}

	collision_mean = float(total_collisions) / float(N);
	transmission_mean = float(num_transmissions) / float(N);

	for(vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
		Node * node = &(*it);
		collision_sq += float(node->num_collisions - collision_mean) * float(node->num_collisions - collision_mean);
		transmission_sq += float(node->num_transmissions - transmission_mean) * float(node->num_transmissions - transmission_mean);
	}

	float channel_utilization = float(num_transmissions) * float(L) / float(T);
	float channel_idle = 1.0 - float(channel_utilization);

	cout << collision_mean << endl;
	cout << transmission_mean << endl;
	cout << "answers: " << endl;
	cout << channel_utilization << endl;
	cout << channel_idle << endl;
	cout << num_collisions << endl;
	cout << transmission_sq/float(N) << endl;
	cout << collision_sq/float(N) << endl;


	return 0;
}

