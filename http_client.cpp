/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>

#include <arpa/inet.h>

#define MAXDATASIZE 100 // max number of bytes we can get at once 
using namespace std;

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

string parse_args(char * argv[], string * port_ptr){
	string port = "80";
	int colon_pos = 0, slash_pos = 0, hostname_end = 0;;
	string filename = "", hostname = "";
	string arg = string(argv[1]);
	if (arg.compare(0, 7, "http://") != 0){
		fprintf(stderr, "parse_args: use http:// in arg");
		exit(1);
	}

	if ((slash_pos = arg.find("/", 7)) > 0){
		filename.append(arg.substr(slash_pos, arg.length())); 
		if ((colon_pos = arg.find(":", 7)) > 7){
			port = arg.substr(colon_pos + 1, slash_pos - colon_pos - 1).c_str();
			hostname_end = colon_pos;
		}
		else {
			hostname_end = slash_pos;
		}	
	}
	else{
		fprintf(stderr, "parse_args: no file specified");
		exit(1);
	}
	
	hostname.append(arg.substr(7, hostname_end - 7));

	cout << "hostname: " << hostname << endl;
	cout << "port: " << port << endl;
	cout << "filename: " << filename << endl;
	
	string output = "";
	output.append("GET " + filename + " HTTP/1.1\r\n"); 
	output.append("User-Agent: MP1/1\r\n");
	output.append("Accept: /\r\n");
	output.append("Accept-Encoding: ide\r\n");
	output.append("Host: " + hostname + ":" + port + "\r\n");
	
	*port_ptr = port;
	return output;
}

int main(int argc, char *argv[])
{
	int sockfd, numbytes;  
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];
	string * port;

	if (argc != 2) {
	    fprintf(stderr, "usage: client hostname[:port]/path/to/file\n");
	    exit(1);
	}
	string header = parse_args(argv, port);
	cout << "header:\n" << header << endl;
	cout << "port: " << *port << endl;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(argv[1], (*port).c_str(), &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); // all done with this structure
	
	if (send(sockfd, header.c_str(), header.length(), 0) == -1){
		perror("send");
	}

	if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	    perror("recv");
	    exit(1);
	}

	buf[numbytes] = '\0';

	printf("client: received '%s'\n",buf);

	close(sockfd);

	return 0;
}
