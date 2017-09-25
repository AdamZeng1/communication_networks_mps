#include "request.h"
#include <iostream>

#define MAXDATASIZE 100
using namespace std;

Request::Request(string buf){
	this->request_type = find_request_type(buf);
	if (!(this->request_type == "GET" || this->request_type == "POST")){
		this->request_type = "-1";
	}
	this->filename = find_filename(buf);
	this->http_protocol = find_http_protocol(buf);
	this->user_agent = find_user_agent(buf);
	this->host = find_host(buf);
	this->accept = find_accept(buf);
	this->accept_encoding = find_accept_encoding(buf);
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
	int new_line_1_idx = buf.find("\n");
	return buf.substr(second_space_idx+1, new_line_1_idx - second_space_idx - 1 - 1);
}

string Request::find_user_agent(string buf){
	int pre_idx = buf.find("User-Agent: ");
	if (pre_idx == -1)
		return "";
	int start_idx = buf.find(" ", pre_idx);
	int end_idx = buf.find("\n", start_idx + 1);
	return buf.substr(start_idx + 1, end_idx - start_idx - 1);
}

string Request::find_host(string buf){
	int pre_idx = buf.find("Host: ");
	if (pre_idx == -1)
		return "";
	int start_idx = buf.find(" ", pre_idx);
	int end_idx = buf.find(":", start_idx + 1);
	return buf.substr(start_idx + 1, end_idx - start_idx - 1);
}

string Request::find_accept(string buf){
	int pre_idx = buf.find("Accept: ");
	if (pre_idx == -1)
		return "";
	int start_idx = buf.find(" ", pre_idx);
	int end_idx = buf.find("\n", start_idx + 1);
	return buf.substr(start_idx + 1, end_idx - start_idx - 1);
}

string Request::find_accept_encoding(string buf){
	int pre_idx = buf.find("Accept-Encoding: ");
	if (pre_idx == -1)
		return "";
	int start_idx = buf.find(" ", pre_idx);
	int end_idx = buf.find("\n", start_idx + 1);
	return buf.substr(start_idx + 1, end_idx - start_idx - 1);
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

string Request::get_host(){
	return this->host;
}

string Request::get_accept(){
	return this->accept;
}

string Request::get_accept_encoding(){
	return this->accept_encoding;
}


