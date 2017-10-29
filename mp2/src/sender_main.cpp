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
#include <time.h>


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
#include <errno.h>
#define MTU 1400 

using namespace std;

struct sockaddr_in si_other;
int s;
socklen_t slen;

int send_base = 0;
int last_ack = 0;
int cwnd = 1;

void diep(char *s) {
    perror(s);
    exit(1);
}

string pack_int_to_bytes(int x) {
    string out;
    for (int shift = 24; shift >= 0; shift = shift - 8){
        out.push_back(((char) (x >> shift) & 0xFF));
    }
    return out;
}

void bytes_to_string(string * recv_pkt, char * buf, int idx, int num_bytes){
    for (int i = idx; i < num_bytes; i++){
        recv_pkt->push_back(buf[i]);
    }
}

int bytes_to_int(string bytes){
    int num = 0;
    for (int i = 0; i != 4; i++) {
        num += int(bytes[i] << (24 - i * 8));
    }
    return num;
}

void reliablyTransfer(char* hostname, unsigned short int hostUDPport, char* filename, unsigned long long int bytesToTransfer) {
    //Open the file
    ifstream inFile;
    inFile.open(filename);

    if(!inFile){
        diep( (char *) "file");
        exit(1);
    }

    stringstream file_stream;
    file_stream << inFile.rdbuf();
    string file_string = file_stream.str();
    inFile.close();

    if (bytesToTransfer > file_string.size())
        bytesToTransfer = file_string.size();

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

    struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 30000; //100ms
	setsockopt(s,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(struct timeval));

    string packet;
    int sentBytes = 0, recvBytes = 0;
    char ack_buf[4];
    int cur_ack = -1;
    string ack_pkt;
    while(true){
    	string sequence_message = pack_int_to_bytes(6969);
    	packet = sequence_message + pack_int_to_bytes(bytesToTransfer);
    	if ((sentBytes = sendto(s, packet.data(), packet.length(), 0, (struct sockaddr *) &si_other, slen)) == -1) {
                diep( (char *) "send");
                exit(1);
        }//send file size
        if ((recvBytes = recvfrom(s, ack_buf, 4 , 0, (struct sockaddr *) &si_other, &slen)) == -1) {
        	if (EAGAIN | EWOULDBLOCK){
        		cwnd = 1;
        		continue;
        	}
            perror("recvfrom");
            exit(1);
        }
        bytes_to_string(&ack_pkt, ack_buf, 0, recvBytes);
        cur_ack = bytes_to_int(ack_pkt); 
        if (cur_ack == 6969)
        	break;
    }

    cur_ack = -1;
    while (send_base < bytesToTransfer) {
        int packets_in_window = 0, transferred_bytes = 0;
        while (packets_in_window != cwnd) {
            int seq_number = send_base + transferred_bytes;
            cout << "seq " << seq_number << endl;
            if (seq_number > bytesToTransfer){
            	cout << "here";
                break;
            }

            string sequence_message = pack_int_to_bytes(seq_number);
            if (bytesToTransfer < seq_number + MTU){
            	packet = sequence_message + file_string.substr(seq_number, bytesToTransfer - seq_number);
            }
           	else{
           		cout << "this happens " << endl;
            	packet = sequence_message + file_string.substr(seq_number, MTU);
           	}
            
            cout << packet << endl;
            //cout << packet.length();
            if ((sentBytes = sendto(s, packet.data(), packet.length(), 0, (struct sockaddr *) &si_other, slen)) == -1) {
                diep( (char *) "send");
                exit(1);
            }

            transferred_bytes += sentBytes - 4;
            cout << "t bytes: " << transferred_bytes << endl;
            packets_in_window++;
        }

        int loop_end = transferred_bytes + send_base;

        while(cur_ack < loop_end) {
            if ((recvBytes = recvfrom(s, ack_buf, 4 , 0, (struct sockaddr *) &si_other, &slen)) == -1) {
            	if (EAGAIN | EWOULDBLOCK){
        			cwnd = 1;
        			continue;
        		}
                perror("recvfrom");
                exit(1);
            }

            ack_pkt = "";
            bytes_to_string(&ack_pkt, ack_buf, 0, recvBytes);
            cur_ack = bytes_to_int(ack_pkt); 
            cout << "ack received: "<< cur_ack << " send base: " << send_base << " cwnd: " << cwnd << " loop_end " << loop_end << endl;
            if (cur_ack > send_base){
                cwnd++;
                send_base = cur_ack;
            }
        }
        //cout << "here" << endl;
    }
    /* Send data and receive acknowledgements on s*/

    //printf("Closing the socket\n");
    close(s);
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
