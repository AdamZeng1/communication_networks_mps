#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <sstream> 
#include <streambuf>

#include "graph.h"

using namespace std;

void getTopoNumbers(string line, int * src_id, int * dest_id, int * cost);
void getMsgNumbers(string line, int * src_id, int * dest_id, string * msg);
void getChangeNumbers(string line, int * src_id, int * dest_id, int * cost_change);
int parseTopology(char * filename, Graph * g);
int parseMessages(char * filename);
int parseChanges(char * filename);

#endif