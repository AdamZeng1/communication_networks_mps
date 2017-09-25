#ifndef REQUEST_H
#define REQUEST_H

#include <string> 

using namespace std;

class Request {
	public:
		Request(string buf);
		string find_request_type(string buf);
		string find_filename(string buf);
		string find_http_protocol(string buf);
		string find_user_agent(string buf);
		string get_request_type();
		string get_filename();
		string get_http_protocol();
		string get_user_agent();
		
	private:
		int first_space_idx;
		int second_space_idx;
		int new_line_1_idx;
		string request_type;
		string filename;
		string http_protocol;
		string user_agent;
};

#endif
