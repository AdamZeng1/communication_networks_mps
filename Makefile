all:
	make client server
client:
	gcc http_client.cpp -o http_client
server:
	gcc http_server.cpp -o http_server
clean:
	rm http_client http_server
