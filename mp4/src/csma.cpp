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
		cout << node.backoff << endl;
	}

	for (int clk = 0; clk < T; clk++){

	}

	return 0;
}

