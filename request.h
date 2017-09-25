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
		string find_host(string buf);
		string find_accept(string buf);
		string find_accept_encoding(string buf);

		string get_request_type();
		string get_filename();
		string get_http_protocol();
		string get_user_agent();
		string get_host();
		string get_accept();
		string get_accept_encoding();
		
	private:
		int first_space_idx;
		int second_space_idx;

		string request_type;
		string filename;
		string http_protocol;
		string user_agent;
		string host;
		string accept;
		string accept_encoding;
};

#endif
