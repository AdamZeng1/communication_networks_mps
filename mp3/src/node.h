#include <string.h>
#include <stdlib.h>
#include <map>

using namespace std; 

class Node {
	int id;
	map<int, int> distances;

	public:
		Node(int);
		int get_id();
		int add_edge(int, int);
		int remove_edge(int);
		int update_edge(int, int);
		int get_cost(int);

};