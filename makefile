all: server client

server:unix_socket_server.c
	gcc $< -o $@ -O2 

client:unix_socket_client.c
	gcc $< -o $@ -O2
