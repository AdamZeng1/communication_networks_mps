/* 
 * File:   receiver_main.c
 * Author: 
 *
 * Created on
 */

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
#include <fstream>
#include <iostream>
#define MTU 10
#define MYPORT "4950"
using namespace std;

struct sockaddr_in si_me, si_other;
int s;
socklen_t slen;

void diep(char *s) {
	perror(s);
	exit(1);
}

string pack_int_to_bytes(int num) {
	string out;
	for (int shift = 24; shift >= 0; shift = shift - 8){
		out.push_back(((char) (num >> shift) & 0xFF));
	}
	return out;
}

int pack_bytes_to_int(string bytes){
	int num = 0;
	for (int i = 0; i != 4; i++) {
		num += int(bytes[i] << (24 - i * 8));
	}
	return num;
}

void bytes_to_string(string * recv_pkt, char * buf, int idx, int num_bytes){
	for (int i = idx; i < num_bytes; i++){
		recv_pkt->push_back(buf[i]);
	}
	return;
}

int sendACK(int ack_num){
	int sentBytes = 0;
	cout << "sending ACK: " << ack_num << endl;
	string packet = pack_int_to_bytes(ack_num);
	if ((sentBytes = sendto(s, packet.data(), packet.length(), 0, (struct sockaddr *) &si_other, slen)) == -1) {
		diep( (char *) "send");
		return -1;
	}
	return 1;
}

void add_to_file(char* destinationFile, string msg){
	cout << "appending to file: " << msg << endl;
	ofstream file;
	file.open(destinationFile, ofstream::out | ofstream::app);
	file << msg;
	file.close();
	return;
}

void init_file(char * destinationFile){
	ofstream file;
	file.open(destinationFile);
	file << "";
	file.close();
}

void reliablyReceive(unsigned short int myUDPport, char* destinationFile) {
	slen = sizeof (si_other);
	char buf[MTU+4];
	char recv_buf[MTU+4];
	string recv_pkt;
	string msg;

	int recvBytes = 0;
	int seq_num = -1;

	int expected_seq_num = 0;
	int expected_file_size = 0;

	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		diep((char *)"socket");

	memset((char *) &si_me, 0, sizeof (si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(myUDPport);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    printf("Now binding\n");
	if (bind(s, (struct sockaddr*) &si_me, sizeof (si_me)) == -1)
		diep((char *)"bind");

	init_file(destinationFile);

	/* Now receive data and send acknowledgements */ 
	while(1){
		if ((recvBytes = recvfrom(s, recv_buf, MTU+4 , 0, (struct sockaddr *) &si_other, &slen)) == -1) {
			perror("recvfrom");
			exit(1);
		}
		recv_pkt = "";
		bytes_to_string(&recv_pkt, recv_buf, 0, recvBytes);
		cout << recv_pkt << endl;
		cout << "bytes received: " << recvBytes << endl;
		seq_num = pack_bytes_to_int(recv_pkt.substr(0, 4));
		cout << "sequence number: " << seq_num << endl;

		msg = recv_pkt.substr(4,recvBytes-4);

		if (seq_num == 6969){
			expected_file_size = pack_bytes_to_int(recv_pkt.substr(4, 4));
			cout << "exepcted file size: " << expected_file_size << endl;
			sendACK(6969);
			if (expected_file_size == 0){
				break;
			}
			continue;
		}

		if (seq_num == expected_seq_num){
			// deliver data: to file
			cout << "msg size: " << msg.length() << endl;
			add_to_file(destinationFile, msg);
			expected_seq_num += recvBytes - 4; // don't count seq_num in recvBytes
			sendACK(expected_seq_num);
			cout << "expected seq num: " << expected_seq_num << endl;
			if (expected_seq_num == expected_file_size)
				break;
		}
		else {
			// discard packet, no buffering
			sendACK(expected_seq_num);
		}
	}

	close(s);
	cout << destinationFile << " received!" << endl;;
	return;
}

int main(int argc, char** argv) {
	unsigned short int udpPort;
	if (argc != 3) {
		fprintf(stderr, "usage: %s UDP_port filename_to_write\n\n", argv[0]);
		exit(1);
	}
	udpPort = (unsigned short int) atoi(argv[1]);
	reliablyReceive(udpPort, argv[2]);
	return 0;
}

