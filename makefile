all: server client select

server:unix_socket_server.c
	gcc $< -o $@ -O2 

client:unix_socket_client.c
	gcc $< -o $@ -O2

select:unix_socket_select.c
	gcc $< -o $@ -O2
