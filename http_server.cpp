/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "http_server.h"
#include "request.h"
#include <iostream>
#include <string>
#include <sstream> 
#include <fstream>
#include <streambuf>


#define BACKLOG 10	 // how many pending connections queue will hold
#define MAXDATASIZE 100

using namespace std;


void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

string to_string(int i){
    stringstream ss;
    ss << i;
    return ss.str();
}

string get_date(){
 	time_t now = time(0);
   	char* dt = ctime(&now);
	string date = (string)dt;
	if (!date.empty() && date[date.length()-1] == '\n') {
    		date.erase(date.length()-1);
	}
	return date;
}

string get_file_type(string filename){
	int type_idx = filename.find(".");
	return filename.substr(type_idx + 1);
}

string encode_response(string status_code, Request * request, string file_string){
	string response = "";
	string status_msg = "";
	if (status_code == "200"){
		status_msg = "OK";
	}
	else if (status_code == "404"){
		status_msg = "Not Found";
	}
	
  
	string date = "Date: " + get_date();
	string file_type = get_file_type(request->get_filename());

	// form header
	response.append(request->get_http_protocol()+" "+status_code+" "+status_msg+"\r\n");
	response.append(date + "\r\n");
	response.append("Content-Length: " + to_string(file_string.size()) + "\r\n");
	response.append("Content-Type: text/html\r\n");

	response.append("\r\n");

	// form document
	response.append(file_string + "\r\n");

	return response;
}

int main(int argc, char *argv[])
{
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;
	
	int numbytes;
	char buf[MAXDATASIZE];

	if(argc != 2){
		exit(1);
	}
	
	char * PORT = argv[1];
	cout << "server will use port number: " << PORT << endl;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	printf("server: waiting for connections...\n");

	

	while(1) {  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s);
		printf("server: got connection from %s\n", s);

		if (!fork()) { // this is the child process
			close(sockfd); // child doesn't need the listener
			
			if ((numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0))== -1){
				perror("recv");
			}

			buf[numbytes] = '\0';
			Request * request = new Request(string(buf));
			
			// prints request class attributes
			
			cout << "request type: " << request->get_request_type() << endl;
			cout << "filename: " << request->get_filename() << endl;
			cout << "http protocol: " << request->get_http_protocol() << endl;
			cout << "user agent: " << request->get_user_agent() << endl;
			cout << "host: " << request->get_host() << endl;

			string file_string = "";
			string status_code = "";
			string homepath = getenv("HOME");
			string filepath = homepath + "/" + request->get_filename();
			ifstream req_file(filepath.c_str());


			// change status_code to 400 for bad request
			// check request->filename, host, etc.

			if (!req_file){
				status_code = "404";
			}
			else{
				stringstream file_stream;
				file_stream << req_file.rdbuf();
				file_string = file_stream.str();
				status_code = "200";
			}
			
			string response = encode_response(status_code, request, file_string);
			cout << "RESPONSE: \n" << response << endl;
			

			// send file + error code + other info back to client

			if (send(new_fd, (const void *)response.c_str(), response.size(), 0) == -1){
				perror("send");
			}
			close(new_fd);
			exit(0);
		}
	
		close(new_fd);  // parent doesn't need this
	}

	return 0;
}

