COMPILERFLAGS = -g -Wall -Wextra -Wno-sign-compare -std=c++11

all:
	make csma
csma:
	g++ $(COMPILERFLAGS) src/csma.cpp src/node.cpp -o csma

clean:
	rm -f csma
