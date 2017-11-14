COMPILERFLAGS = -g -Wall -Wextra -Wno-sign-compare

all:
	make linkstate distvec 
distvec:
	g++ $(COMPILERFLAGS) src/distvec.cpp src/parser.cpp src/graph.cpp src/node.cpp -o distvec
linkstate:
	g++ $(COMPILERFLAGS) src/linkstate.cpp src/parser.cpp src/graph.cpp src/node.cpp -o linkstate
clean:
	rm distvec linkstate
