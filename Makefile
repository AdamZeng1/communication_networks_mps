all:
	make client server
client:
	g++ http_client.cpp -o http_client
server:
	g++ -std=c++11 -pthread http_server.cpp request.cpp -o http_server
clean:
	rm http_client http_server
