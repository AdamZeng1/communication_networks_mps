/* 
 * File:   sender_main.c
 * Author: 
 *
 * Created on 
 */

//user added
#include <iostream>
#include <string>
#include <sstream> 
#include <fstream>
#include <streambuf>



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>

#define MTU 1500 

using namespace std;

struct sockaddr_in si_other;
int s, slen;

int send_base = 0;
int cwnd = 1;


void diep(char *s) {
    perror(s);
    exit(1);
}


void reliablyTransfer(char* hostname, unsigned short int hostUDPport, char* filename, unsigned long long int bytesToTransfer) {
    //Open the file
    char buf[1500];
   	ifstream inFile;
   	inFile.open(filename);

   	if(!inFile){
   		diep( (char *) "file");
   		exit(1);
   	}

   	stringstream file_stream;
    file_stream << inFile.rdbuf();
    string file_string = file_stream.str();

	/* Determine how many bytes to transfer */

    slen = sizeof (si_other);

    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        diep( (char *) "socket");

    memset((char *) &si_other, 0, sizeof (si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(hostUDPport);
    if (inet_aton(hostname, &si_other.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    string packet;
    int transferred_bytes = 0, sentBytes = 0;
    while (transferred_bytes != bytesToTransfer) {
	    int packets_in_window = 0;
	    packet = file_string.substr(transferred_bytes, MTU);
	    while (packet.length() && packets_in_window != cwnd) {
		    
		    if ((sentBytes = sendto(s, packet.c_str(), packet.length(), 0, (struct sockaddr *) &si_other, slen) == -1)) {
				diep( (char *) "send");
				exit(1);
			}
			transferred_bytes += sentBytes;
			packets_in_window++;
		}
	}
	/* Send data and receive acknowledgements on s*/

    printf("Closing the socket\n");
    close(s);
    inFile.close();
    return;

}

/*
 * 
 */
int main(int argc, char** argv) {

    unsigned short int udpPort;
    unsigned long long int numBytes;

    if (argc != 5) {
        fprintf(stderr, "usage: %s receiver_hostname receiver_port filename_to_xfer bytes_to_xfer\n\n", argv[0]);
        exit(1);
    }
    udpPort = (unsigned short int) atoi(argv[2]);
    numBytes = atoll(argv[4]);



    reliablyTransfer(argv[1], udpPort, argv[3], numBytes);


    return (EXIT_SUCCESS);
}


