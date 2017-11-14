#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <sstream> 
#include <streambuf>

using namespace std;

void getTopoNumbers(string line, int * src_id, int * dest_id, int * cost);
void getMsgNumbers(string line, int * src_id, int * dest_id, string * msg);
int parseTopology(char * filename);
int parseMessages(char * filename);

#endif