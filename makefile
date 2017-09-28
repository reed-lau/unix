all: helper server client select socket_select socket_client

helper:helper.c
	gcc $< -o $@.o -O2 -c -I./

server:unix_socket_server.c
	gcc $< -o $@ -O2 helper.o -I./

client:unix_socket_client.c
	gcc $< -o $@ -O2 helper.o -I./

select:unix_socket_select.c
	gcc $< -o $@ -O2 helper.o -I./

socket_select:socket_select.c
	gcc $< -o $@ -O2 helper.o -I./

socket_client:socket_client.c
	gcc $< -o $@ -O2 helper.o -I./
