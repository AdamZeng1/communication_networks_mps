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
#define MTU 1500
#define MYPORT "4950"
using namespace std;

struct sockaddr_in si_me, si_other;
int s;
socklen_t slen;

void diep(char *s) {
	perror(s);
	exit(1);
}
/*
int sendACK(int seq_num){
	string packet;
	packet = to_string(seq_num);
	if ((int sentBytes = sendto(s, packet.data, packet.length(), 0, (struct sockaddr *) &si_other, slen) == -1)) {
		diep( (char *) "send");
		return -1;
	}
	return 1;
}*/

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
}

void reliablyReceive(unsigned short int myUDPport, char* destinationFile) {
	slen = sizeof (si_other);
	char buf[MTU+4];
	char recv_buf[MTU+4];
	string recv_pkt;

	int recvBytes = 0;

	int rwnd = 1500;
	int rcvbase = 0;
	int seq_num = 6969;

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


	/* Now receive data and send acknowledgements */ 

	while(1){
		if ((recvBytes = recvfrom(s, recv_buf, MTU+4 , 0, (struct sockaddr *) &si_other, &slen)) == -1) {
			perror("recvfrom");
			exit(1);
		}
		bytes_to_string(&recv_pkt, recv_buf, 0, recvBytes);
		cout << recv_pkt << endl;
		cout << "bytes received: " << recvBytes << endl;
		
		seq_num = pack_bytes_to_int(recv_pkt.substr(0, 4));
		cout << "sequence number: " << seq_num << endl;

		if (seq_num == 0){
			expected_file_size = pack_bytes_to_int(recv_pkt.substr(4, 4));
			cout << "exepcted file size: " << expected_file_size << endl;
		}

		/*else if (rcvbase == expected_file_size){
			// send ack for packet
			if (sendACK(expected_seq_num)){
				break;
			}
		}
		else if (seq_num == expected_seq_num){
			// deliver data: to file? to temporary data structure? 
			// sndpkt = make_pkt(expectedseqnum,ACK,chksum)
			// udt_send(sndpkt)
			expected_seq_num += 1;
		}
		else {
				// no receiver buffering, discard packet
				// re-ack pkt w/ highest in-order seq # 
				if (sendACK(expected_seq_num)){
				break;
			}
		}*/
		break;
	}

	close(s);
	ofstream file;
	file.open(destinationFile);
	file << buf;
	file.close();
	printf("%s received.\n", destinationFile);
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

