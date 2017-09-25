#include "request.h"
#include <iostream>

#define MAXDATASIZE 100
using namespace std;

Request::Request(string buf){
	this->request_type = find_request_type(buf);
	this->filename = find_filename(buf);
	this->http_protocol = find_http_protocol(buf);
	this->user_agent = find_user_agent(buf);
}

string Request::find_request_type(string buf){
	first_space_idx = buf.find(" ");
	return buf.substr(0, first_space_idx);
}

string Request::find_filename(string buf){
	second_space_idx = buf.find(" ", first_space_idx+1);
	return buf.substr(first_space_idx+2, second_space_idx - first_space_idx - 1 - 1);
}

string Request::find_http_protocol(string buf){
	new_line_1_idx = buf.find("\n");
	return buf.substr(second_space_idx+1, new_line_1_idx - second_space_idx);
}

string Request::find_user_agent(string buf){
	int pre_idx = new_line_1_idx + 1;
	int start_idx = buf.find(" ", pre_idx+1);
	int end_idx = buf.find("\n", start_idx+1);
	return buf.substr(start_idx + 1, end_idx - start_idx);
}

string Request::get_request_type(){
	return this->request_type;
}

string Request::get_filename(){
	return this->filename;
}

string Request::get_http_protocol(){
	return this->http_protocol;
}

string Request::get_user_agent(){
	return this->user_agent;
}
